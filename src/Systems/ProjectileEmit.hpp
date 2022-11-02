#pragma once

#include "../Components/BoxCollider.hpp"
#include "../Components/CameraFollow.hpp"
#include "../Components/Projectile.hpp"
#include "../Components/ProjectileEmitter.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/Sprite.hpp"
#include "../Components/Transform.hpp"
#include "../Events/KeyPressed.hpp"
#include <SDL2/SDL.h>
#include <entt/entt.hpp>

struct ProjectileEmitSystem {

  void OnKeyPressed(KeyPressedEvent &event, entt::registry &registry) {
    if (event.symbol == SDLK_SPACE) {
      auto view = registry.view<ProjectileEmitterComponent, TransformComponent,
                                RigidBodyComponent>();

      for (auto entity : view) {
        if (registry.any_of<CameraFollowComponent>(entity)) {
          const auto projectileEmitter =
              view.get<ProjectileEmitterComponent>(entity);
          const auto transform = view.get<TransformComponent>(entity);
          const auto rigidbody = view.get<RigidBodyComponent>(entity);

          // Check if it's time to re-emit a new projectile
          glm::vec2 projectilePosition = transform.position;
          if (registry.any_of<SpriteComponent>(entity)) {
            auto viewSprite =
                registry.view<ProjectileComponent, TransformComponent,
                              SpriteComponent>();
            const auto sprite = viewSprite.get<SpriteComponent>(entity);
            projectilePosition.x += (transform.scale.x * sprite.width / 2);
            projectilePosition.y += (transform.scale.y * sprite.height / 2);
          }
          // If parent entity direction is controlled by the keyboard keys,
          // modify the direction of the projectile accordingly
          glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
          int directionX = 0;
          int directionY = 0;
          if (rigidbody.velocity.x > 0)
            directionX = +1;
          if (rigidbody.velocity.x < 0)
            directionX = -1;
          if (rigidbody.velocity.y > 0)
            directionY = +1;
          if (rigidbody.velocity.y < 0)
            directionY = -1;
          projectileVelocity.x =
              projectileEmitter.projectileVelocity.x * directionX;
          projectileVelocity.y =
              projectileEmitter.projectileVelocity.y * directionY;

          // Createnew projectile entity and add it to the world
          entt::entity projectile = registry.create();
          registry.emplace<TransformComponent>(projectile, projectilePosition,
                                               glm::vec2(1.0, 1.0), 0.0);
          registry.emplace<RigidBodyComponent>(projectile, projectileVelocity);
          registry.emplace<SpriteComponent>(projectile, "bullet-texture", 4, 4,
                                            4);
          registry.emplace<BoxColliderComponent>(projectile, 4, 4);
          registry.emplace<ProjectileComponent>(
              projectile, projectileEmitter.isFriendly,
              projectileEmitter.hitPercentDamage,
              projectileEmitter.projectileDuration);
        }
      }
    }
  }

  void Update(entt::registry &registry) {
    auto view = registry.view<ProjectileEmitterComponent, TransformComponent>();

    for (auto entity : view) {
      auto &projectileEmitter =
          registry.get<ProjectileEmitterComponent>(entity);
      const auto transform = registry.get<TransformComponent>(entity);

      if (projectileEmitter.repeatFrequency == 0) {
        continue;
      }

      // Check if it's time to re-emit a new projectile
      if (static_cast<int>(SDL_GetTicks()) -
                  projectileEmitter.lastEmissionTime >
              projectileEmitter.repeatFrequency &&
          projectileEmitter.isFriendly == false) {

        glm::vec2 projectilePosition = transform.position;
        if (registry.any_of<SpriteComponent>(entity)) {
          auto viewSprite =
              registry.view<ProjectileComponent, TransformComponent,
                            SpriteComponent>();
          const auto sprite = viewSprite.get<SpriteComponent>(entity);
          projectilePosition.x += (transform.scale.x * sprite.width / 2);
          projectilePosition.y += (transform.scale.y * sprite.height / 2);
        }

        // Add a new projectile entity to the registry
        entt::entity projectile = registry.create();
        registry.emplace<TransformComponent>(projectile, projectilePosition,
                                             glm::vec2(1.0, 1.0), 0.0);
        registry.emplace<RigidBodyComponent>(
            projectile, projectileEmitter.projectileVelocity);
        registry.emplace<SpriteComponent>(projectile, "bullet-texture", 4, 4,
                                          4);
        registry.emplace<BoxColliderComponent>(projectile, 4, 4);
        registry.emplace<ProjectileComponent>(
            projectile, projectileEmitter.isFriendly,
            projectileEmitter.hitPercentDamage,
            projectileEmitter.projectileDuration);

        // Update the projectile emitter component last emiision to the
        // current milliseconds
        projectileEmitter.lastEmissionTime = SDL_GetTicks();
      }
    }
  }
};

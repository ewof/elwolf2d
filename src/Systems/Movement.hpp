#pragma once

#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include "../Events/Collision.hpp"
#include "../Components/Transform.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/Enemy.hpp"
#include "../Components/Obstacle.hpp"
#include "../Components/Sprite.hpp"
#include "../Components/KeyboardControlled.hpp"

struct MovementSystem {

  void onCollision(CollisionEvent &event, entt::registry& registry) {
    entt::entity a = event.a;
    entt::entity b = event.b;
    // Logger::Log("Collision event emitted: " + std::to_string(a.GetId()) + "
    // and " + std::to_string(b.GetId()));

    if (registry.any_of<EnemyComponent>(a) && registry.any_of<ObstacleComponent>(b)) {
      OnEnemyHitsObstacle(a, b, registry); // "a" is the enemy, "b" is the obstacle
    }
    if(registry.any_of<EnemyComponent>(b) && registry.any_of<ObstacleComponent>(a)) {
      OnEnemyHitsObstacle(b, a, registry); // "b" is the enemy, "a" is the obstacle
    }
  }

  void OnEnemyHitsObstacle(entt::entity enemy, entt::entity obstacle, entt::registry& registry) {
    if (registry.any_of<RigidBodyComponent>(enemy) &&
        registry.any_of<SpriteComponent>(enemy)) {

      auto view = registry.view<SpriteComponent, RigidBodyComponent>();

      auto &sprite = view.get<SpriteComponent>(enemy);
      auto &rigidbody = view.get<RigidBodyComponent>(enemy);

      if (rigidbody.velocity.x != 0) {
        rigidbody.velocity.x *= -1;
        sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL
                                                     : SDL_FLIP_NONE;
      }
      if (rigidbody.velocity.y != 0) {
        rigidbody.velocity.y *= -1;
        sprite.flip =
            (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
      }
    }
  }

  void Update(double deltaTime, entt::registry& registry, int mapHeight, int mapWidth) {
    auto view = registry.view<TransformComponent, RigidBodyComponent>();

    // Loop all entities that the system is interested in
    for (auto entity : view) {
      // Update entity position based on its velocity
      auto &transform = view.get<TransformComponent>(entity);
      auto rigidbody = view.get<RigidBodyComponent>(entity);

      transform.position.x += rigidbody.velocity.x * deltaTime;
      transform.position.y += rigidbody.velocity.y * deltaTime;

      // Prevent the main player from moving outside the map boundaries
      if (registry.any_of<KeyboardControlledComponent>(entity)) {
        int paddingLeft = 10;
        int paddingTop = 10;
        int paddingRight = 50;
        int paddingBottom = 50;
        transform.position.x = transform.position.x < paddingLeft
                                   ? paddingLeft
                                   : transform.position.x;
        transform.position.x =
            transform.position.x > mapWidth - paddingRight
                ? mapWidth - paddingRight
                : transform.position.x;
        transform.position.y = transform.position.y < paddingTop
                                   ? paddingTop
                                   : transform.position.y;
        transform.position.y =
            transform.position.y > mapHeight - paddingBottom
                ? mapHeight - paddingBottom
                : transform.position.y;
      }

      // Dunno if i want this or not
      int margin = 0;

      // Cull out-of-bounds entities
      if ((transform.position.x < -margin ||
           transform.position.x > mapWidth + margin ||
           transform.position.y < -margin ||
           transform.position.y > mapHeight + margin) &&
          !registry.any_of<KeyboardControlledComponent>(entity)) {
        registry.destroy(entity);
      }
    }
  }
};
#pragma once

#include "../Components/KeyboardControlled.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/Sprite.hpp"
#include "../Events/KeyPressed.hpp"
#include <SDL2/SDL.h>
#include <entt/entt.hpp>

struct KeyboardControlSystem {
  void OnKeyPressed(KeyPressedEvent &event, entt::registry &registry) {
    auto view = registry.view<SpriteComponent, RigidBodyComponent,
                              KeyboardControlledComponent>();

    for (auto entity : view) {
      const auto keyboardcontrol =
          view.get<KeyboardControlledComponent>(entity);
      auto &sprite = view.get<SpriteComponent>(entity);
      auto &rigidbody = view.get<RigidBodyComponent>(entity);
      switch (event.symbol) {
      case SDLK_UP:
      case SDLK_w:
        rigidbody.velocity = keyboardcontrol.upVelocity;
        sprite.srcRect.y = sprite.height * 0;
        break;
      case SDLK_RIGHT:
      case SDLK_d:
        rigidbody.velocity = keyboardcontrol.rightVelocity;
        sprite.srcRect.y = sprite.height * 1;
        break;
      case SDLK_DOWN:
      case SDLK_s:
        rigidbody.velocity = keyboardcontrol.downVelocity;
        sprite.srcRect.y = sprite.height * 2;
        break;
      case SDLK_LEFT:
      case SDLK_a:
        rigidbody.velocity = keyboardcontrol.leftVelocity;
        sprite.srcRect.y = sprite.height * 3;
        break;
      }
    }
  }
};
#pragma once

#include "../Components/Animation.hpp"
#include "../Components/Sprite.hpp"
#include <entt/entt.hpp>

struct AnimationSystem {

  void Update(entt::registry &registry) {

    auto view = registry.view<AnimationComponent, SpriteComponent>();

    for (auto entity : view) {
      auto &animation = view.get<AnimationComponent>(entity);
      auto &sprite = view.get<SpriteComponent>(entity);

      animation.currentFrame = ((SDL_GetTicks() - animation.startTime) *
                                animation.frameSpeedRate / 1000) %
                               animation.numFrames;
      sprite.srcRect.x = animation.currentFrame * sprite.width;
    }
  }
};
#pragma once

#include "../Components/CameraFollow.hpp"
#include "../Components/Transform.hpp"
#include <SDL2/SDL.h>
#include <entt/entt.hpp>

struct CameraMovementSystem {

  void Update(SDL_Rect &camera, entt::registry &registry, int mapWidth,
              int mapHeight, int windowWidth, int windowHeight) {
    auto view = registry.view<TransformComponent, CameraFollowComponent>();

    for (auto entity : view) {
      auto transform = view.get<TransformComponent>(entity);

      if (transform.position.x + (camera.w / 2) < mapWidth) {
        camera.x = transform.position.x - (windowWidth / 2);
      }

      if (transform.position.y + (camera.h / 2) < mapHeight) {
        camera.y = transform.position.y - (windowHeight / 2);
      }

      // Keep camera rectangle view inside the screen limits
      camera.x = camera.x < 0 ? 0 : camera.x;
      camera.y = camera.y < 0 ? 0 : camera.y;
      camera.x = camera.x > camera.w ? camera.w : camera.x;
      camera.y = camera.y > camera.h ? camera.h : camera.y;
    }
  }
};
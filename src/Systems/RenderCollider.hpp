#pragma once

#include <entt/entt.hpp>
#include "../Components/Transform.hpp"
#include "../Components/BoxCollider.hpp"
#include <SDL2/SDL.h>

struct RenderColliderSystem{
    void Update(entt::registry& registry, SDL_Renderer* renderer, SDL_Rect& camera) {
        auto view = registry.view<TransformComponent,BoxColliderComponent>();

        for(auto entity: view) {
            const auto transform = view.get<TransformComponent>(entity);
            const auto collider = view.get<BoxColliderComponent>(entity);

            SDL_Rect colliderRect = {
                static_cast<int>(transform.position.x + collider.offset.x -
                                 camera.x),
                static_cast<int>(transform.position.y + collider.offset.y -
                                 camera.y),
                static_cast<int>(collider.width * transform.scale.x),
                static_cast<int>(collider.height * transform.scale.y)};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &colliderRect);
        }
    }
};
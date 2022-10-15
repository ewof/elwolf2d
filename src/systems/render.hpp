#pragma once

#include <vector>
#include <memory>
#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include "../AssetStore/AssetStore.h"
#include "../components/sprite.hpp"
#include "../components/transform.hpp"

struct render {

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera, entt::registry& registry) {
        struct RenderableEntity {
            transform transformComponent;
            sprite spriteComponent;
        };

        std::vector<RenderableEntity> renderableEntities;

        auto view = registry.view<const transform, sprite>();

        for(auto entity: view) {
            RenderableEntity renderableEntity;
            renderableEntity.transformComponent = view.get<transform>(entity);
            renderableEntity.spriteComponent = view.get<sprite>(entity);


            // Don't render non-fixed entities outside the camera view
			if((renderableEntity.transformComponent.position.x + (renderableEntity.transformComponent.scale.x * renderableEntity.spriteComponent.width) < camera.x || renderableEntity.transformComponent.position.x > camera.x + camera.w || renderableEntity.transformComponent.position.y + (renderableEntity.transformComponent.scale.y * renderableEntity.spriteComponent.height) < camera.y || renderableEntity.transformComponent.position.y > camera.y + camera.h) && !renderableEntity.spriteComponent.isFixed) {
				continue;
			}

            renderableEntities.emplace_back(renderableEntity);
        }

        // Sort the vector by the z-index value
        std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
            return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
        });

        // Loop all entities that the system is interested in
        for (auto entity: renderableEntities) {
            const auto transform = entity.transformComponent;
            const auto sprite = entity.spriteComponent;

            // Set the source rectangle of our original sprite texture
            SDL_Rect srcRect = sprite.srcRect;

            // Set the destination rectangle with the x,y position to be rendered
            SDL_Rect dstRect = {
                static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
                static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)
            };

            // Draw the texture on the destination renderer
            SDL_RenderCopyEx(
                renderer,
                assetStore->GetTexture(sprite.assetId),
                &srcRect,
                &dstRect,
                transform.rotation,
                NULL,
                sprite.flip
            );
        }
    }

};
#pragma once

#include "../AssetStore/AssetStore.hpp"
#include "../Components/Health.hpp"
#include "../Components/Transform.hpp"
#include "../Components/Sprite.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <entt/entt.hpp>

struct RenderHealthBarSystem {

  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &AssetStore,
              SDL_Rect &camera, entt::registry &registry) {

    auto view = registry.view<const TransformComponent, HealthComponent>();

    for (auto entity : view) {
       const auto transform = registry.get<TransformComponent>(entity);
       const auto sprite = registry.get<SpriteComponent>(entity);
       const auto health = registry.get<HealthComponent>(entity);

        // Draw a health bar with the correct color for the percentage
        SDL_Color healthBarColor = {255,255,255};

        if(health.healthPercentage >=0 && health.healthPercentage < 40) {
            // 0-40 = red
            healthBarColor = {255,0,0};
        }
        if (health.healthPercentage >= 40 && health.healthPercentage < 80) {
            // 40-80 = yellow
            healthBarColor = {255,255,0};
        }
        if (health.healthPercentage >= 80 && health.healthPercentage <= 100) {
            // 80-100 = green
            healthBarColor = {0,255,0};
        }

        // Position the health bar indicator in the middle-bottom part of the entity spire
        int healthBarWidth = 15;
        int healthBarHeight = 3;
        double healthBarPosX = (transform.position.x + (sprite.width * transform.scale.x)) - camera.x;
        double healthBarPosY = (transform.position.y) - camera.y;

        SDL_Rect healthBarRectangle = {
            static_cast<int>(healthBarPosX),
            static_cast<int>(healthBarPosY),
            static_cast<int>(healthBarWidth * (health.healthPercentage / 100.0)),
            static_cast<int>(healthBarHeight)
        };

        SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, 255);
        SDL_RenderFillRect(renderer, &healthBarRectangle);

        // Render the health percentage text label indicator
        std::string healthText = std::to_string(health.healthPercentage);
        SDL_Surface* surface = TTF_RenderText_Blended(AssetStore->GetFont("pico8-font-5"), healthText.c_str(), healthBarColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        int labelWidth = 0;
        int labelHeight = 0;
        SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);
        SDL_Rect healthBarTextRectangle = {
            static_cast<int>(healthBarPosX),
            static_cast<int>(healthBarPosY) + 5,
            labelWidth,
            labelHeight,
        };

        SDL_RenderCopy(renderer, texture, NULL, &healthBarTextRectangle);

        SDL_DestroyTexture(texture);

    }
  }
};
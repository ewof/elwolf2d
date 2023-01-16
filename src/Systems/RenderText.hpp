#pragma once

#include "../AssetStore/AssetStore.hpp"
#include "../Components/TextLabel.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <entt/entt.hpp>

struct RenderTextSystem {

  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &AssetStore,
              SDL_Rect &camera, entt::registry &registry) {

    auto view = registry.view<const TextLabelComponent>();

    for (auto entity : view) {
       const auto textlabel = registry.get<TextLabelComponent>(entity);
       
        SDL_Surface* surface = TTF_RenderText_Blended(AssetStore->GetFont(textlabel.assetId),textlabel.text.c_str(),textlabel.color);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        int labelWidth = 0;
        int labelHeight = 0;

        SDL_QueryTexture(texture,NULL, NULL, &labelWidth, &labelHeight);

        SDL_Rect dstRect = {static_cast<int>(textlabel.position.x - (textlabel.isFixed ? 0 : camera.x)),static_cast<int>(textlabel.position.y  - (textlabel.isFixed ? 0 : camera.y)),labelWidth,labelHeight};

        SDL_RenderCopy(renderer, texture, NULL, &dstRect);

        SDL_DestroyTexture(texture);
    }
  }
};
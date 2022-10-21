#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <string>

struct SpriteComponent {
  std::string assetId;
  int width;
  int height;
  int zIndex;
  bool isFixed;
  SDL_RendererFlip flip;
  SDL_Rect srcRect;

  SpriteComponent(std::string assetId = "", int width = 0, int height = 0,
                  int zIndex = 0, bool isFixed = false, int srcRectX = 0,
                  int srcRectY = 0) {
    this->assetId = assetId;
    this->width = width;
    this->height = height;
    this->zIndex = zIndex;
    this->isFixed = isFixed;
    this->flip = SDL_FLIP_NONE;
    this->srcRect = {srcRectX, srcRectY, width, height};
  }
};
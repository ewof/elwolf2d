#pragma once

#include "../AssetStore/AssetStore.hpp"
#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <memory>
#include <sol/sol.hpp>

class LevelLoader {

public:
  LevelLoader();
  ~LevelLoader();

  void LoadLevel(sol::state &lua,
                 const std::unique_ptr<entt::registry> &registry,
                 const std::unique_ptr<AssetStore> &AssetStore,
                 SDL_Renderer *renderer, std::string level);
};
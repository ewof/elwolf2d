#pragma once

#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>
#include <memory>
#include <sol/sol.hpp>
#include <entt/entt.hpp>

class LevelLoader {

	public:
        LevelLoader();
        ~LevelLoader();

		void LoadLevel(sol::state& lua, const std::unique_ptr<entt::registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, std::string level);

};
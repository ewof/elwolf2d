#include "./LevelLoader.h"
#include <spdlog/spdlog.h>

LevelLoader::LevelLoader() {
	spdlog::info("LevelLoader constructor called!");
}

LevelLoader::~LevelLoader() {
	spdlog::info("LevelLoader destructor called!");
}

void LevelLoader::LoadLevel(sol::state& lua, const std::unique_ptr<entt::registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, std::string level) {
	sol::load_result script = lua.load_file(level);

	//Checks syntax of script, doesn't execute it
	if(!script.valid()) {
		sol::error err = script;
		std::string errorMessage = err.what();
		spdlog::error("LUA Error: " + errorMessage);
		return;
	}

	lua.script_file(level);
}
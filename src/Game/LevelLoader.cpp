#include "LevelLoader.hpp"
#include <spdlog/spdlog.h>

LevelLoader::LevelLoader() { spdlog::info("LevelLoader constructor called!"); }

LevelLoader::~LevelLoader() { spdlog::info("LevelLoader destructor called!"); }

void LevelLoader::LoadLevel(sol::state *lua,
                            entt::registry &registry,
                            const std::unique_ptr<AssetStore> &AssetStore,
                            SDL_Renderer *renderer, int level) {

  std::string token = "../assets/scripts/MainMenu.lua";

  if(level != -1) {
    token = std::string("../assets/scripts/Level" + std::to_string(level) + ".lua");
  }

  sol::load_result script = lua->load_file(token);

  // Checks syntax of script, doesn't execute it
  if (!script.valid()) {
    sol::error err = script;
    std::string errorMessage = err.what();
    spdlog::error("LUA Error: " + errorMessage);
    return;
  }

  lua->script_file(token);
}
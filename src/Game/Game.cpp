#include "Game.hpp"
#include "../../libs/imgui/backends/imgui_impl_sdl.h"
#include "../../libs/imgui/backends/imgui_impl_sdlrenderer.h"
#include "../../libs/imgui/imgui.h"
#include "../Components/Sprite.hpp"
#include "../Components/Transform.hpp"
#include "LevelLoader.hpp"
#include "spdlog/spdlog.h"
#include <fstream>

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;
const char *Game::name;
LevelLoader loader;

Game::Game() {
  isRunning = false;
  isDebug = false;
  registry = std::make_unique<entt::registry>();
  assetStore = std::make_unique<AssetStore>();
  renderSystem = std::make_unique<render>();
  spdlog::info("Game constructor called!");
}

Game::~Game() { spdlog::info("Game destructor called!"); }

void Game::Init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    spdlog::error("Error initializing SDL");
    return;
  }

  Game::name = "elwolf2d";
  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(1, &displayMode);

  windowWidth = displayMode.w;
  windowHeight = displayMode.h;
  window = SDL_CreateWindow(Game::name, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                            SDL_WINDOW_BORDERLESS);
  if (!window) {
    spdlog::error("Error creating SDL winodw");
    return;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    spdlog::error("Error creating SDL renderer");
    return;
  }

  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer_Init(renderer);

  camera.x = 0;
  camera.y = 0;
  camera.w = windowWidth;
  camera.h = windowHeight;

  isRunning = true;
}

void Game::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
      isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        isRunning = false;
      }
      if (event.key.keysym.sym == SDLK_BACKQUOTE) {
        isDebug = !isDebug;
      }
      break;
    }
  }
}

void Game::Update() {
  int timeToWait =
      MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
    SDL_Delay(timeToWait);
  }

  double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

  millisecsPreviousFrame = SDL_GetTicks();

  // Update systems
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  renderSystem->Update(renderer, assetStore, camera, *registry);
  if (isDebug) {
    // Update RenderColliderSystem
    // Update RenderGUI system
  }

  SDL_RenderPresent(renderer);
}

void Game::Run() {
  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
  loader.LoadLevel(lua, registry, assetStore, renderer,
                   "../assets/scripts/Level3.lua");

  while (isRunning) {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::Destory() {
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
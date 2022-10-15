#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <sol/sol.hpp>
#include "../AssetStore/AssetStore.h"
#include "../systems/render.hpp"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {

	private:
		bool isRunning;
		bool isDebug;
		int millisecsPreviousFrame = 0;
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Rect camera;

		sol::state lua;		
		std::unique_ptr<entt::registry> registry;
		std::unique_ptr<AssetStore> assetStore;

		std::unique_ptr<render> renderSystem;

	public:
		Game();
		~Game();
		void Init();
		void Run();
		void ProcessInput();
		void Update();
		void Render();
		void Destory();

		static int windowWidth;
		static int windowHeight;
		static int mapWidth;
		static int mapHeight;
		static const char* name;

};


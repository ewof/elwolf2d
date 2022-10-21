#pragma once

#include "../AssetStore/AssetStore.hpp"
#include "../Systems/Animation.hpp"
#include "../Systems/CameraMovement.hpp"
#include "../Systems/Collision.hpp"
#include "../Systems/Damage.hpp"
#include "../Systems/KeyboardControl.hpp"
#include "../Systems/Movement.hpp"
#include "../Systems/Render.hpp"
#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <memory>
#include <sol/sol.hpp>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {

private:
  bool isRunning;
  bool isDebug;
  int millisecsPreviousFrame = 0;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Rect camera;

  sol::state lua;
  std::unique_ptr<entt::registry> registry;
  std::unique_ptr<AssetStore> assetStore;
  entt::delegate<void(CollisionEvent &event, entt::registry &registry)>
      damageSystemDelegate{}; // I dont think delegates is the best way to do
                              // these, Damage sys could  prolly bc merged to
                              // Collision system but i'm just porting for now,
                              // will prolly change later
  entt::delegate<void(CollisionEvent &event, entt::registry &registry)>
      movementSystemDelegate{};
  entt::delegate<void(KeyPressedEvent &event, entt::registry &registry)>
      keyboardControlSystemDelagate{};

  std::unique_ptr<RenderSystem> renderSystem;
  std::unique_ptr<AnimationSystem> animationSystem;
  std::unique_ptr<CollisionSystem> collisionSystem;
  std::unique_ptr<DamageSystem> damageSystem;
  std::unique_ptr<KeyboardControlSystem> keyboardControlSystem;
  std::unique_ptr<CameraMovementSystem> cameraMovementSystem;
  std::unique_ptr<MovementSystem> movementSystem;

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
  static const char *name;
};

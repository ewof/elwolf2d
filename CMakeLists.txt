cmake_minimum_required(VERSION 3.24.2)
project(elwolfengine VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 23)

find_package(spdlog REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})


set(SOURCES
	assets/scripts/Level1.lua
	assets/scripts/Level2.lua
	assets/scripts/Level3.lua
	src/AssetStore/AssetStore.hpp
	src/AssetStore/AssetStore.cpp

    src/Events/Collision.hpp
    src/Events/KeyPressed.hpp

	src/Components/Animation.hpp
	src/Components/BoxCollider.hpp
    src/Components/CameraFollow.hpp
    src/Components/Enemy.hpp
    src/Components/Health.hpp
    src/Components/KeyboardControlled.hpp
    src/Components/Projectile.hpp
    src/Components/ProjectileEmitter.hpp
    src/Components/RigidBody.hpp
    src/Components/Script.hpp
    src/Components/Sprite.hpp
    src/Components/TextLabel.hpp
    src/Components/Transform.hpp
    src/Components/Obstacle.hpp

	src/Systems/Render.hpp
    src/Systems/Animation.hpp
    src/Systems/Collision.hpp
    src/Systems/Damage.hpp
    src/Systems/KeyboardControl.hpp
    src/Systems/CameraMovement.hpp
    src/Systems/Movement.hpp
    src/Systems/ProjectileEmit.hpp

	src/Game/LevelLoader.hpp
	src/Game/LevelLoader.cpp
	src/Game/Game.hpp
	src/Game/Game.cpp
	src/Main.cpp
)

set(NAME elwolf2d)
set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

set(IMGUI_DIR ${LIBS_DIR}/imgui)
add_library(IMGUI STATIC)

target_sources(IMGUI
	PRIVATE
		${IMGUI_DIR}/imgui_demo.cpp
		${IMGUI_DIR}/imgui_draw.cpp
		${IMGUI_DIR}/imgui_tables.cpp
		${IMGUI_DIR}/imgui_widgets.cpp
		${IMGUI_DIR}/imgui.cpp

	PRIVATE
		${IMGUI_DIR}/backends/imgui_impl_sdlrenderer.cpp
		${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
)

target_include_directories(IMGUI
	PUBLIC ${IMGUI_DIR}
	PUBLIC ${IMGUI_DIR}/backends
	PUBLIC ${SDL2_DIR}/include
)

target_link_libraries(IMGUI PUBLIC SDL2main SDL2 ${CMAKE_DL_LIBS})

SET (LUA_DIR ${LIBS_DIR}/lua)
SET (SOL2_DIR ${LIBS_DIR}/sol2/include/sol)

add_library(
    LUA STATIC
    ${LUA_DIR}/lapi.c
    ${LUA_DIR}/lauxlib.c
    ${LUA_DIR}/lbaselib.c
    ${LUA_DIR}/lcode.c
    ${LUA_DIR}/lcorolib.c
    ${LUA_DIR}/lctype.c
    ${LUA_DIR}/ldblib.c
    ${LUA_DIR}/ldebug.c
    ${LUA_DIR}/ldo.c
    ${LUA_DIR}/ldump.c
    ${LUA_DIR}/lfunc.c
    ${LUA_DIR}/lgc.c
    ${LUA_DIR}/linit.c
    ${LUA_DIR}/liolib.c
    ${LUA_DIR}/llex.c
    ${LUA_DIR}/lmathlib.c
    ${LUA_DIR}/lmem.c
    ${LUA_DIR}/loadlib.c
    ${LUA_DIR}/lobject.c
    ${LUA_DIR}/lopcodes.c
    ${LUA_DIR}/loslib.c
    ${LUA_DIR}/lparser.c
    ${LUA_DIR}/lstate.c
    ${LUA_DIR}/lstring.c
    ${LUA_DIR}/lstrlib.c
    ${LUA_DIR}/ltable.c
    ${LUA_DIR}/ltablib.c
    ${LUA_DIR}/ltm.c
    ${LUA_DIR}/lua.c
    ${LUA_DIR}/lundump.c
    ${LUA_DIR}/lutf8lib.c
    ${LUA_DIR}/lvm.c
    ${LUA_DIR}/lzio.c
)

add_executable(${NAME})
target_sources(${NAME} PUBLIC ${SOURCES})
target_include_directories(${NAME}
	PUBLIC libs/entt/src
	PRIVATE ${SOL2_DIR}
	PRIVATE ${LUA_DIR}
)
target_link_libraries(${NAME} IMGUI LUA ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
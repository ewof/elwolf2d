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
	src/AssetStore/AssetStore.h
	src/AssetStore/AssetStore.cpp
	src/components/sprite.hpp
	src/components/transform.hpp
	src/systems/render.hpp
	src/game/game.h
	src/game/game.cpp
	src/main.cpp
)

set(NAME elwolfengine)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
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

add_executable(${NAME})
target_sources(${NAME} PUBLIC ${SOURCES})
target_include_directories(elwolfengine
	PUBLIC libs/entt/src
)
target_link_libraries(${NAME} IMGUI ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
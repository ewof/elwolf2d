#pragma once
#include "../Components/Transform.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/Sprite.hpp"
#include "../Components/BoxCollider.hpp"
#include "../Components/ProjectileEmitter.hpp"
#include "../Components//Health.hpp"
#include "../Game/LevelLoader.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <entt/entt.hpp>
#include <memory>
#include <vector>
#include <sol.hpp>
#include <spdlog/spdlog.h>

#include "../../libs/imgui/backends/imgui_impl_sdl.h"
#include "../../libs/imgui/backends/imgui_impl_sdlrenderer.h"
struct RenderGUISystem {

  void Update(entt::registry& registry, std::vector<std::string> textureList, SDL_Rect camera, const std::unique_ptr<AssetStore>& assetStore, LevelLoader& loader, SDL_Renderer* renderer, sol::state *lua) {
			
            ImGui_ImplSDLRenderer_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
            
			if (ImGui::Begin("Load Level")) {
				static int level = 1;
				ImGui::InputInt("Level", &level);
				if (ImGui::Button("Load")) {
					loader.LoadLevel(lua,registry,assetStore,renderer,level);
				}
			}

			ImGui::End();

			if (ImGui::Begin("Spawn enemies")) {
				static int xPos = 0;
				static int yPos = 0;
				static int xScale = 1;
				static int yScale = 1;
				static float rotation = 0;

				static double xVel = 0.0;
				static double yVel = 0.0;

				static std::string sprite = "tank-image";
				static int width = 32;
				static int height = 32;
				static int zIndex = 1;
				static bool isFixed = false;
				static int srcRectX = 0;
				static int srcRectY = 0;

				static int xBoxCollider = 32;
				static int yBoxCollider = 32;

				static float projectileSpeed = 100.0f;
				static float projectileAngle = 0.0f;
				static float repeatFrequency = 1.0f;
				static float projectileDuration = 3.0f;
				static int hitPercentDamage = 10;

				static int health = 100;
                
				if(ImGui::CollapsingHeader("Transform"),ImGuiTreeNodeFlags_DefaultOpen) {
					ImGui::InputInt("X Pos", &xPos);
					ImGui::InputInt("Y Pos", &yPos);

					ImGui::SliderInt("X Scale", &xScale,0,10);
					ImGui::SliderInt("Y Scale", &yScale,0,10);

					ImGui::SliderAngle("Rotation (Degrees)", &rotation, 0.0f, 360.0);
				}

				ImGui::Spacing();

				if(ImGui::CollapsingHeader("Rigid Body"),ImGuiTreeNodeFlags_DefaultOpen) {
					ImGui::InputDouble("X Velocity", &xVel);
					ImGui::InputDouble("Y Velocity", &yVel);
				}

				ImGui::Spacing();

				if(ImGui::CollapsingHeader("Sprite"),ImGuiTreeNodeFlags_DefaultOpen) {
					ImGui::Text("Select Sprite");
					{
						int textureListLength = textureList.size();
						const char* sprites[textureListLength];
						for (int i = 0; i < textureListLength; i++) {
							sprites[i] = textureList[i].c_str();
						}
						static int item_current = 0;
						ImGui::Combo("##combo", &item_current, sprites, IM_ARRAYSIZE(sprites));
						sprite = sprites[item_current];
						ImGui::SameLine();
					}

					ImGui::Spacing();
					ImGui::InputInt("Width", &width);
					ImGui::InputInt("Height", &height);
					ImGui::InputInt("zIndex", &zIndex);
					ImGui::Text("Is Fixed?");
					{
						const char* bools[] = {"False","True"};
						static int item_current_two = 0;
						ImGui::Combo("##combo2", &item_current_two, bools, IM_ARRAYSIZE(bools));
						if(item_current_two == 0) {
							isFixed = false;
						} else {
							isFixed = true;
						}
					}
					ImGui::InputInt("Source Rect X", &srcRectX);
					ImGui::InputInt("Source Rect Y", &srcRectY);
				}

				ImGui::Spacing();

				if(ImGui::CollapsingHeader("Box Collider"),ImGuiTreeNodeFlags_DefaultOpen) {
					ImGui::InputInt("X Box Collider", &xBoxCollider);
					ImGui::InputInt("Y Box Collider", &yBoxCollider);
				}

				ImGui::Spacing();

				if(ImGui::CollapsingHeader("Projectile Emitter"),ImGuiTreeNodeFlags_DefaultOpen) {
					ImGui::InputFloat("Projectile Speed (px/sec)", &projectileSpeed);
					ImGui::SliderAngle("Projectile Angle (degrees)", &projectileAngle,0.0f,360.0f);

					ImGui::InputFloat("Repeat Frequency (sec)", &repeatFrequency);
					ImGui::InputFloat("Projectile Duration (sec)", &projectileDuration);
					ImGui::InputInt("Hit Percent Damage", &hitPercentDamage);
				}

				ImGui::Spacing();

				if(ImGui::CollapsingHeader("Health"),ImGuiTreeNodeFlags_DefaultOpen) {
					ImGui::SliderInt("Starting health %",&health,0,100);
				}

				ImGui::Spacing();

				if (ImGui::Button("Spawn")) {
                    entt::entity enemy = registry.create();
                    registry.emplace<TransformComponent>(enemy, glm::vec2(xPos, yPos), glm::vec2(xScale,yScale), glm::degrees(rotation));
                    registry.emplace<RigidBodyComponent>(enemy, glm::vec2(xVel,yVel));
                    registry.emplace<SpriteComponent>(enemy,sprite, width, height, zIndex, isFixed, srcRectX, srcRectY);
                    registry.emplace<BoxColliderComponent>(enemy,xBoxCollider,yBoxCollider);
                    registry.emplace<ProjectileEmitterComponent>(enemy,glm::vec2(cos(projectileAngle)*projectileSpeed, sin(projectileAngle)*projectileSpeed), repeatFrequency*1000, projectileDuration*1000, hitPercentDamage, false);
                    registry.emplace<BoxColliderComponent>(enemy,xBoxCollider,yBoxCollider);
					registry.emplace<HealthComponent>(enemy,health);

				}
			}
			ImGui::End();

			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
			ImGui::SetNextWindowPos(ImVec2(10,10), ImGuiCond_Always, ImVec2(0,0));
			ImGui::SetNextWindowBgAlpha(0.50f);
			auto io = ImGui::GetIO();
			if(ImGui::Begin("Coordinates",NULL,windowFlags)) {
				ImGui::Text("Cursor Coordinates (x=%.1f, y=%.1f)",io.MousePos.x + camera.x,io.MousePos.y + camera.y);
			}

			ImGui::End();
			ImGui::Render();
			ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		}
};
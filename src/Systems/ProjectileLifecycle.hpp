#pragma once

#include <entt/entt.hpp>
#include "../Components/Projectile.hpp"
#include <SDL2/SDL.h>

struct ProjectileLifecycleSystem{

    void Update(entt::registry& registry) {
        auto view = registry.view<ProjectileComponent>();

        for(auto entity: view) {
            auto projectile = view.get<ProjectileComponent>(entity); 

            if(static_cast<int>(SDL_GetTicks()) - projectile.startTime > projectile.duration) {
                registry.destroy(entity);
            }
        }
    }

};
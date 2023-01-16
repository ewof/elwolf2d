#pragma once

#include "../AssetStore/AssetStore.hpp"
#include "../Components/Script.hpp"
#include "../Components/Transform.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/Animation.hpp"
#include "../Components/ProjectileEmitter.hpp"
#include <tuple>
#include <spdlog/spdlog.h>
#include <entt/entt.hpp>

std::tuple<double, double> GetEntityPosition(entt::entity entity, entt::registry &registry) {
    auto view = registry.view<const TransformComponent>();
    if (registry.any_of<TransformComponent>(entity)) {
        const auto transform = registry.get<TransformComponent>(entity);
        return std::make_tuple(transform.position.x, transform.position.y);
    } else {
        spdlog::error("Trying to get the position of an entity that has no transform component");
        return std::make_tuple(0.0, 0.0);
    }
}

std::tuple<double, double> GetEntityVelocity(entt::entity entity, entt::registry &registry) {
    if (registry.any_of<RigidBodyComponent>(entity)) {
        const auto rigidbody = registry.get<RigidBodyComponent>(entity);
        return std::make_tuple(rigidbody.velocity.x, rigidbody.velocity.y);
    } else {
        spdlog::error("Trying to get the velocity of an entity that has no rigidbody component");
        return std::make_tuple(0.0, 0.0);
    }
}

void SetEntityPosition(entt::entity entity, entt::registry &registry, double x, double y) {
    if (registry.any_of<TransformComponent>(entity)) {
        auto& transform = registry.get<TransformComponent>(entity);
        transform.position.x = x;
        transform.position.y = y;
    } else {
        spdlog::error("Trying to set the position of an entity that has no transform component");
    }
}

void SetEntityVelocity(entt::entity entity, entt::registry &registry, double x, double y) {
    if (registry.any_of<RigidBodyComponent>(entity)) {
        auto& rigidbody = registry.get<RigidBodyComponent>(entity);
        rigidbody.velocity.x = x;
        rigidbody.velocity.y = y;
    } else {
        spdlog::error("Trying to set the velocity of an entity that has no rigidbody component");
    }
}

void SetEntityRotation(entt::entity entity, entt::registry &registry, double angle) {
    if (registry.any_of<TransformComponent>(entity)) {
        auto& transform = registry.get<TransformComponent>(entity);
        transform.rotation = angle;
    } else {
        spdlog::error("Trying to set the rotation of an entity that has no transform component");
    }
}

void SetEntityAnimationFrame(entt::entity entity, entt::registry &registry, int frame) {
    if (registry.any_of<AnimationComponent>(entity)) {
        auto& animation = registry.get<AnimationComponent>(entity);
        animation.currentFrame = frame;
    } else {
        spdlog::error("Trying to set the animation frame of an entity that has no animation component");
    }
}

void SetProjectileVelocity(entt::entity entity, entt::registry &registry, double x, double y) {
    if (registry.any_of<ProjectileEmitterComponent>(entity)) {
        auto& projectileEmitter = registry.get<ProjectileEmitterComponent>(entity);
        projectileEmitter.projectileVelocity.x = x;
        projectileEmitter.projectileVelocity.y = y;
    } else {
        spdlog::error("Trying to set the projectile velocity of an entity that has no projectile emitter component");
    }
}

void KillEntity(entt::entity entity, entt::registry &registry) {
    registry.destroy(entity);
}

struct ScriptSystem {

    void CreateLuaBindings(sol::state& lua) {
        // Create the "entity" usertype so Lua knows what an entity is
        lua.new_usertype<entt::entity>(
            "entity"
        );

        // Create all the bindings between C++ and Lua functions
        lua.set_function("get_position", GetEntityPosition);
        lua.set_function("get_velocity", GetEntityVelocity);
        lua.set_function("set_position", SetEntityPosition);
        lua.set_function("set_velocity", SetEntityVelocity);
        lua.set_function("set_rotation", SetEntityRotation);
        lua.set_function("set_projectile_velocity", SetProjectileVelocity);
        lua.set_function("set_animation_frame", SetEntityAnimationFrame);
        lua.set_function("kill_entity", KillEntity);
    }

  void Update(double deltaTime, int ellapsedTime, entt::registry &registry) {

    auto view = registry.view<const ScriptComponent>();

    for (auto entity : view) {
       const auto script = registry.get<ScriptComponent>(entity);
       
        script.func(entity,deltaTime,ellapsedTime);
    }
  }
};
#pragma once

#include "../Components/BoxCollider.hpp"
#include "../Components/CameraFollow.hpp"
#include "../Components/Enemy.hpp"
#include "../Components/Health.hpp"
#include "../Components/Projectile.hpp"
#include "../Events/Collision.hpp"
#include <entt/entt.hpp>

struct DamageSystem {

  void onCollision(CollisionEvent &event, entt::registry &registry) {
    entt::entity a = event.a;
    entt::entity b = event.b;

    if (registry.any_of<ProjectileComponent>(a) &&
        registry.any_of<CameraFollowComponent>(b)) {
      OnProjectileHitsPlayer(registry, a,
                             b); // "a" is the projectile, "b" is the player
    }

    if (registry.any_of<ProjectileComponent>(b) &&
        registry.any_of<CameraFollowComponent>(a)) {
      OnProjectileHitsPlayer(registry, b,
                             a); // "a" is the projectile, "b" is the player
    }

    if (registry.any_of<ProjectileComponent>(a) &&
        registry.any_of<EnemyComponent>(b)) {
      OnProjectileHitsEnemy(registry, a,
                            b); // "a" is the projectile, "b" is the player
    }

    if (registry.any_of<ProjectileComponent>(b) &&
        registry.any_of<EnemyComponent>(a)) {
      OnProjectileHitsEnemy(registry, b,
                            a); // "a" is the projectile, "b" is the player
    }
  }

  void OnProjectileHitsEnemy(entt::registry &registry, entt::entity projectile,
                             entt::entity enemy) {
    auto viewProjectileComponent = registry.view<ProjectileComponent>();

    const auto projectileComponent =
        viewProjectileComponent.get<ProjectileComponent>(projectile);

    // Only damage the enemy if projectile is friendly
    if (projectileComponent.isFriendly) {
      auto viewHealthComponent = registry.view<HealthComponent>();
      auto &health = viewHealthComponent.get<HealthComponent>(enemy);

      // Subtract from enemy health
      health.healthPercentage -= projectileComponent.hitPercentDamage;

      // Kills the enemy if health reaches zero
      if (health.healthPercentage <= 0) {
        registry.destroy(enemy);
      }

      // Destroy projectile
      registry.destroy(projectile);
    }
  }

  void OnProjectileHitsPlayer(entt::registry &registry, entt::entity projectile,
                              entt::entity player) {
    auto viewProjectileComponent = registry.view<ProjectileComponent>();

    const auto projectileComponent =
        viewProjectileComponent.get<ProjectileComponent>(projectile);

    if (!projectileComponent.isFriendly) {
      // Reduce the health of the player by the projectile hitPercentDamage
      auto viewHealthComponent = registry.view<HealthComponent>();
      auto &health = viewHealthComponent.get<HealthComponent>(player);

      // Subtract the health of the player
      health.healthPercentage -= projectileComponent.hitPercentDamage;

      // Kills the player when health reaches zero
      if (health.healthPercentage <= 0) {
        registry.destroy(player);
      }

      // Kill the projectile
      registry.destroy(projectile);
    }
  }
};
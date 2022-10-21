#pragma once

#include "../Components/BoxCollider.hpp"
#include "../Components/Transform.hpp"
#include "../Events/Collision.hpp"
#include "../Game/Game.hpp"
#include <entt/entt.hpp>

struct CollisionSystem {
  void
  Update(entt::registry &registry,
         entt::delegate<void(CollisionEvent &event, entt::registry &registry)>
             &damageSystemDelagate,
         entt::delegate<void(CollisionEvent &event, entt::registry &registry)>
             &movementSystemDelegate) {
    auto view = registry.view<TransformComponent, BoxColliderComponent>();

    for (auto i = view.begin(); i != view.end(); i++) {
      entt::entity a = *i;

      auto &aTransform = view.get<TransformComponent>(a);
      auto &aCollider = view.get<BoxColliderComponent>(a);

      // Loop all the entities that still need to be checked (to the right of i)
      for (auto j = i; j != view.end(); j++) {
        entt::entity b = *j;

        // Bypass if we are trying to test the same entity
        if (a == b) {
          continue;
        }

        auto bTransform = view.get<TransformComponent>(b);
        auto bCollider = view.get<BoxColliderComponent>(b);

        // Perform the AABB collision check between entities a and b
        bool collisionHappened = CheckAABBCollision(
            aTransform.position.x + aCollider.offset.x,
            aTransform.position.y + aCollider.offset.y, aCollider.width,
            aCollider.height, bTransform.position.x + bCollider.offset.x,
            bTransform.position.y + bCollider.offset.y, bCollider.width,
            bCollider.height);

        if (collisionHappened) {
          CollisionEvent e = {a, b};
          damageSystemDelagate(e, registry);
          movementSystemDelegate(e, registry);
        }
      }
    }
  }

  bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX,
                          double bY, double bW, double bH) {
    return (aX < bX + bW && aX + aW > bX && aY < bY + bH && aY + aH > bY);
  }
};
#pragma once

#include <glm/glm.hpp>

struct RigidBody{
  glm::vec2 velocity;

  RigidBody(glm::vec2 velocity = glm::vec2(0.0, 0.0)) {
    this->velocity = velocity;
  }
};
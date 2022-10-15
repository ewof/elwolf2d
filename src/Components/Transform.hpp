#pragma once

#include <glm/glm.hpp>

struct Transform {

  glm::vec2 position;
  glm::vec2 scale;
  float rotation;

  Transform(glm::vec2 position = glm::vec2(0, 0),
            glm::vec2 scale = glm::vec2(1, 1), float rotation = 0.0) {
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
  }
};
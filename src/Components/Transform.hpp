#pragma once

#include <glm/glm.hpp>

struct TransformComponent {

  glm::vec2 position;
  glm::vec2 scale;
  float rotation;

  TransformComponent(glm::vec2 position = glm::vec2(0, 0),
                     glm::vec2 scale = glm::vec2(1, 1), float rotation = 0.0) {
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
  }
};
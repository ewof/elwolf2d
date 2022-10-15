#pragma once

struct Health {
  int healthPercentage;

  Health(int healthPercentage = 0) {
    this->healthPercentage = healthPercentage;
  }
};
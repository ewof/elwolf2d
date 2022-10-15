#pragma once

#include <SDL2/SDL.h>

struct Animation {
  int numFrames;
  int currentFrame;
  int frameSpeedRate;
  bool isLoop;
  int startTime;

  Animation(int numFrames = 1, int frameSpeedRate = 1, bool isLoop = true) {
    this->numFrames = numFrames;
    this->currentFrame = 1;
    this->frameSpeedRate = frameSpeedRate;
    this->isLoop = isLoop;
    this->startTime = SDL_GetTicks();
  }
};
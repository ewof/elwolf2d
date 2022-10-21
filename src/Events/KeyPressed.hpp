#pragma once

#include <SDL2/SDL.h>

struct KeyPressedEvent {
    SDL_Keycode symbol;
    KeyPressedEvent(SDL_Keycode symbol): symbol(symbol) {}
};
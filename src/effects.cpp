#pragma once

#include <SDL2/SDL_stdinc.h>

enum class game_effects {
  color,
  tween,
};

int current_effects = -1;

float lerp(float &x, float target) {
  x += (target - x) * 0.1;
  return x;
}

float ease_in_out_quart(float x) {
  return x < 0.5 ? 8 * x * x * x * x : 1 - SDL_pow(-2 * x + 2, 4) / 2;
}
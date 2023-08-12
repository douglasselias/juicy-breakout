#pragma once

#include <SDL2/SDL_stdinc.h>

enum class game_effects {
  color,
  tween,
  sfx,
  bgm,
  ball_smoke_particles,
  ball_trail,
  paddle_confetti,
  screen_shake,
  paddle_eyes, // follow ball blink
  paddle_mouth, // happy when bounce, frown when ball is far away
  hit_stop, // slow-mo when hits the blocks
};

int current_effects = 2;

float lerp(float &x, float target) {
  x += (target - x) * 0.1;
  return x;
}

float ease_in_out_quart(float x) {
  return x < 0.5 ? 8 * x * x * x * x : 1 - SDL_pow(-2 * x + 2, 4) / 2;
}

float ease_in_out_back(float x) {
  const float c1 = 1.70158;
  const float c2 = c1 * 1.525;

  return x < 0.5
             ? (SDL_pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
             : (SDL_pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}
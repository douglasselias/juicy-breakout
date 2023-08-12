#pragma once
#include <cstdint>
#include <cstdio>

#include <string>

#include <SDL2/SDL.h>

#include "effects.cpp"
#include "window.cpp"

typedef struct paddle_entity {
  SDL_FRect dimensions;
} paddle_entity;

paddle_entity create_paddle() {
  const float gap = 20;
  const float width = 150;
  const float height = 40;

  return {
      .dimensions =
          {
              .x = half_window_width - (width / 2),
              .y = window_height - height - gap,
              .w = width,
              .h = height,
          },
  };
}

int t = 0;
void update_paddle(paddle_entity &paddle, float eased_progress, int mouse_x) {
  int delta = SDL_abs(mouse_x - paddle.dimensions.x);
  paddle.dimensions.x = (mouse_x - (paddle.dimensions.w / 2));

  if (current_effects >= (int)game_effects::tween) {
    paddle.dimensions.y =
        eased_progress * (window_height - paddle.dimensions.h - 20);
  }
}

void render_paddle(paddle_entity *paddle) {
  if (current_effects >= (int)game_effects::color)
    SDL_SetRenderDrawColor(renderer, 255, 161, 0, 255);

  render_rect(&paddle->dimensions);

  if (current_effects >= (int)game_effects::color)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

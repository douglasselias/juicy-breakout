#pragma once
#include <cstdint>
#include <cstdio>

#include <string>

#include <SDL2/SDL.h>

#include "effects.cpp"
#include "window.cpp"

typedef struct paddle_input {
  bool left;
  bool right;
} paddle_input;

typedef struct paddle_keys {
  SDL_Keycode left;
  SDL_Keycode right;
} paddle_keys;

typedef struct paddle_entity {
  SDL_FRect dimensions;
  paddle_input current_input;
  paddle_keys keys;
} paddle_entity;

paddle_entity create_paddle(paddle_keys keys) {
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
      .current_input = {.left = false, .right = false},
      .keys = keys,
  };
}

void input_paddle(paddle_entity &paddle, SDL_EventType event_type,
                  SDL_Keycode key_pressed) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
  switch (event_type) {
  case SDL_KEYDOWN:
    if (key_pressed == paddle.keys.left) {
      paddle.current_input.left = true;
    }
    if (key_pressed == paddle.keys.right) {
      paddle.current_input.right = true;
    }
    break;
  case SDL_KEYUP:
    if (key_pressed == paddle.keys.left) {
      paddle.current_input.left = false;
    }
    if (key_pressed == paddle.keys.right) {
      paddle.current_input.right = false;
    }
  }
#pragma clang diagnostic pop
}

void update_paddle(paddle_entity &paddle, float eased_progress) {
  const int paddle_speed = 20;
  const int max_width = window_width - paddle.dimensions.w;

  if (paddle.current_input.left)
    paddle.dimensions.x =
        SDL_clamp(paddle.dimensions.x - paddle_speed, 0, max_width);

  if (paddle.current_input.right)
    paddle.dimensions.x =
        SDL_clamp(paddle.dimensions.x + paddle_speed, 0, max_width);

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

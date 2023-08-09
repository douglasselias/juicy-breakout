#pragma once
#include <SDL2/SDL_render.h>
#include <cstdint>

#include <SDL2/SDL.h>

#include "window.cpp"

struct paddle_size {
  float width;
  float height;
};
using paddle_size = paddle_size;

paddle_size player_size = {
    .width = 20,
    .height = 100,
};

typedef struct player_input {
  uint8_t up;
  uint8_t down;
} player_input;

float initial_y_position = HALF_WINDOW_H - (player_size.height / 2);
float player_gap = 0;

SDL_FPoint player1_position = {
    .x = player_gap,
    .y = initial_y_position,
};
SDL_FPoint player2_position = {
    .x = WINDOW_WIDTH - player_size.width - player_gap,
    .y = initial_y_position,
};

player_input player1_input = {
    .up = 0,
    .down = 0,
};

player_input player2_input = {
    .up = 0,
    .down = 0,
};

int paddle_speed = 10;

int player1_score = 0;
int player2_score = 0;

float clamp_player_position(float position) {
  return SDL_clamp(position, 0, WINDOW_HEIGHT - player_size.height);
}

void render_player1(SDL_Renderer *renderer) {
  SDL_FRect player1 = {
      .x = player1_position.x,
      .y = player1_position.y,
      .w = player_size.width,
      .h = player_size.height,
  };
  SDL_RenderFillRectF(renderer, &player1);
}

void render_player2(SDL_Renderer *renderer) {
  SDL_FRect player2 = {
      .x = player2_position.x,
      .y = player2_position.y,
      .w = player_size.width,
      .h = player_size.height,
  };
  SDL_RenderFillRectF(renderer, &player2);
}
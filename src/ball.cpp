#pragma once
#include <cstdint>

#include <SDL2/SDL.h>

#include "paddle.cpp"
#include "window.cpp"

const uint8_t ball_radius = 10;
const int64_t ball_base_speed = 6;

int64_t ball_speed_x = ball_base_speed;
int64_t ball_speed_y = ball_base_speed;
uint8_t ball_speed_increase_x = 1;
uint8_t ball_speed_increase_y = 1;

int invert_x = 1;
int invert_y = 1;

SDL_FPoint ball_position = {
    .x = HALF_WINDOW_W,
    .y = HALF_WINDOW_H,
};

void update_ball(paddle_entity &paddle1, paddle_entity &paddle2) {
  bool hit_left_wall = ball_position.x - ball_radius + ball_speed_x < 0;
  bool hit_right_wall =
      ball_position.x + ball_radius + ball_speed_x > WINDOW_WIDTH;

  if (hit_left_wall) {
    paddle2.score += 1;
    // reset ball
    ball_position = {
        .x = HALF_WINDOW_W,
        .y = HALF_WINDOW_H,
    };
    ball_speed_x = -ball_base_speed;
    ball_speed_y = ball_base_speed;
  }

  if (hit_right_wall) {
    paddle1.score += 1;
    ball_position = {
        .x = HALF_WINDOW_W,
        .y = HALF_WINDOW_H,
    };
    ball_speed_x = ball_base_speed;
    ball_speed_y = ball_base_speed;
  }

  if (ball_position.y + ball_radius + ball_speed_y > WINDOW_HEIGHT ||
      ball_position.y - ball_radius + ball_speed_y < 0)
    invert_y = invert_y == 1 ? -1 : 1;

  ball_position.x += ball_speed_x * invert_x;
  ball_position.y += ball_speed_y * invert_y;
}

void render_ball(SDL_Renderer *renderer, SDL_FPoint ball_position) noexcept {
  const int radius = ball_radius;
  for (int x = -radius; x <= radius; x += 1) {
    for (int y = -radius; y <= radius; y += 1) {
      if (x * x + y * y <= radius * radius) {
        SDL_RenderDrawPoint(renderer, ball_position.x + x, ball_position.y + y);
      }
    }
  }
}
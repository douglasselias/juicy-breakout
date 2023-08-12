#pragma once
#include <algorithm>

#include <SDL2/SDL_rect.h>
#include <vector>

#include "blocks.cpp"
#include "effects.cpp"
#include "paddle.cpp"
#include "window.cpp"

SDL_FRect create_ball() {
  SDL_FRect ball = {
      .x = half_window_width,
      .y = half_window_height,
      .w = 20,
      .h = 20,
  };
  return ball;
}

SDL_FPoint ball_speed = {
    .x = -0.5,
    .y = 3,
};

bool check_aabb_collision(SDL_FRect rect1, SDL_FRect rect2) {
  return (rect1.x < rect2.x + rect2.w && rect1.x + rect1.w > rect2.x &&
          rect1.y < rect2.y + rect2.h && rect1.y + rect1.h > rect2.y);
}

void ball_paddle_collision(SDL_FRect ball, paddle_entity paddle) {
  if (check_aabb_collision(ball, paddle.dimensions)) {
    ball_speed.y = -ball_speed.y;
  }
}

void ball_blocks_collision(SDL_FRect ball, blocks &blocks) {
  // std::vector<block> blocks_to_remove = {};

  for (block b : blocks) {
    if (check_aabb_collision(ball, b.dimensions)) {
      ball_speed.y = -ball_speed.y;
      // blocks_to_remove.push_back(b);
      blocks.erase(std::remove(blocks.begin(), blocks.end(), b), blocks.end());
    }
  }

  // for (block b : blocks_to_remove) {
  //   blocks.erase(std::remove(blocks.begin(), blocks.end(), b), blocks.end());
  // }
}

void update_ball(SDL_FRect &ball, float delta_time) {
  if (ball.x < 0 || ball.x > window_width)
    ball_speed.x = -ball_speed.x;

  if (ball.y < 0 || ball.y > window_height)
    ball_speed.y = -ball_speed.y;

  ball.x = ball.x + ball_speed.x;
  ball.y = ball.y + ball_speed.y;
}

void render_ball(SDL_FRect ball) {
  if (current_effects >= (int)game_effects::color)
    SDL_SetRenderDrawColor(renderer, 255, 203, 0, 255);

  render_rect(&ball);

  if (current_effects >= (int)game_effects::color)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}
#pragma once
#include <algorithm>

#include <SDL2/SDL_rect.h>
#include <cstdio>
#include <vector>

#include "blocks.cpp"
#include "effects.cpp"
#include "paddle.cpp"
#include "particles.cpp"
#include "window.cpp"

Mix_Chunk *sfx;

bool shake = false;
Uint32 shake_ticks = SDL_GetTicks64();

bool slow_motion = false;
float slow_factor = 0.3f;
float slow_motion_duration = 700;
Uint32 slow_motion_ticks = SDL_GetTicks64();

SDL_FRect create_ball() {
  SDL_FRect ball = {
      .x = half_window_width,
      .y = half_window_height,
      .w = 20,
      .h = 20,
  };
  sfx = load_audio("hit.wav");
  return ball;
}

SDL_FPoint ball_speed = {
    .x = -1,
    .y = 2,
};

void applySlowMotion() {
  ball_speed.x *= slow_factor;
  ball_speed.y *= slow_factor;
}

void restoreSlowMotion() {
  ball_speed.x /= slow_factor;
  ball_speed.y /= slow_factor;
}

bool check_aabb_collision(SDL_FRect rect1, SDL_FRect rect2) {
  return (rect1.x < rect2.x + rect2.w && rect1.x + rect1.w > rect2.x &&
          rect1.y < rect2.y + rect2.h && rect1.y + rect1.h > rect2.y);
}

void ball_paddle_collision(SDL_FRect ball, paddle_entity paddle) {
  if (check_aabb_collision(ball, paddle.dimensions)) {
    ball_speed.y = -ball_speed.y;
    if (current_effects >= (int)game_effects::sfx)
      play_audio(sfx);
    if (current_effects >= (int)game_effects::ball_smoke_particles)
      particles = init_particles({.x = ball.x, .y = ball.y});
  }
}

void ball_blocks_collision(SDL_FRect ball, blocks &blocks) {
  for (block b : blocks) {
    if (check_aabb_collision(ball, b.dimensions)) {
      ball_speed.y = -ball_speed.y;
      if (current_effects >= (int)game_effects::sfx)
        play_audio(sfx);
      if (current_effects >= (int)game_effects::ball_smoke_particles)
        particles = init_particles({.x = ball.x, .y = ball.y});
      if (current_effects >= (int)game_effects::screen_shake) {
        shake = true;
        shake_ticks = SDL_GetTicks64();
      }
      if (current_effects >= (int)game_effects::hit_stop) {
        if (slow_motion == false) {
          slow_motion = true;
          applySlowMotion();
          slow_motion_ticks = SDL_GetTicks64();
        }
      }
      blocks.erase(std::remove(blocks.begin(), blocks.end(), b), blocks.end());
    }
  }
}

std::vector<SDL_FPoint> trail;
void update_ball(SDL_FRect &ball, float delta_time) {
  if (ball.x < 0 || ball.x > window_width) {
    ball_speed.x = -ball_speed.x;
    if (current_effects >= (int)game_effects::sfx)
      play_audio(sfx);
  }

  if (ball.y < 0 || ball.y > window_height) {
    ball_speed.y = -ball_speed.y;
    if (current_effects >= (int)game_effects::sfx)
      play_audio(sfx);
  }

  ball.x += ball_speed.x;
  ball.y += ball_speed.y;

  SDL_FPoint p = {.x = ball.x + ball.w / 2, .y = ball.y};
  trail.push_back(p);
}

void render_ball(SDL_FRect ball) {
  if (current_effects >= (int)game_effects::color)
    SDL_SetRenderDrawColor(renderer, 255, 203, 0, 255);

  render_rect(&ball);

  if (current_effects >= (int)game_effects::ball_trail) {
    for (auto t : trail) {
      SDL_RenderDrawPointF(renderer, t.x, t.y);
    }
    if (trail.size() > 100)
      trail.erase(trail.begin());
  }

  if (current_effects >= (int)game_effects::color)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}
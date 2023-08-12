#pragma once
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <cstdio>

#include <string>

#include <SDL2/SDL.h>

#include "effects.cpp"
#include "window.cpp"

typedef struct paddle_entity {
  SDL_FRect dimensions;
  SDL_FPoint left_eye;
  SDL_FPoint right_eye;
  SDL_FPoint left_pupil;
  SDL_FPoint right_pupil;
  SDL_FPoint mouth;
  bool is_happy;
} paddle_entity;

int eyes_gap = 36;
paddle_entity create_paddle() {
  const float gap = 20;
  const float width = 150;
  const float half_width = width / 2;
  const float height = 40;

  SDL_FRect dimensions = {
      .x = half_window_width - half_width,
      .y = window_height - height - gap,
      .w = width,
      .h = height,
  };

  float y = dimensions.y + 7;
  SDL_FPoint left_eye = {.x = dimensions.x + half_width - eyes_gap, .y = y};
  SDL_FPoint right_eye = {.x = dimensions.x + half_width + eyes_gap, .y = y};

  SDL_FPoint left_pupil = {.x = left_eye.x, .y = y};
  SDL_FPoint right_pupil = {.x = right_eye.x, .y = y};

  SDL_FPoint mouth = {.x = dimensions.x + half_width,
                      .y = dimensions.y + (height / 2)};

  return {
      .dimensions = dimensions,
      .left_eye = left_eye,
      .right_eye = right_eye,
      .left_pupil = left_pupil,
      .right_pupil = right_pupil,
      .mouth = mouth,
      .is_happy = true,
  };
}

float vec_length(SDL_FPoint vec) {
  return SDL_sqrtf(vec.x * vec.x + vec.y * vec.y);
}

SDL_FPoint normalize(SDL_FPoint vec) {
  float length = vec_length(vec);
  if (length != 0) {
    vec.x /= length;
    vec.y /= length;
  }
  return vec;
}

SDL_FPoint sub_vec(SDL_FPoint a, SDL_FPoint b) {
  return {
      .x = a.x - b.x,
      .y = a.y - b.y,
  };
}

void update_paddle(paddle_entity &paddle, float eased_progress, int mouse_x,
                   SDL_FPoint ball) {
  int delta = SDL_abs(mouse_x - paddle.dimensions.x);
  float half_width = paddle.dimensions.w / 2;
  paddle.dimensions.x = mouse_x - half_width;
  paddle.left_eye.x = paddle.dimensions.x + half_width - eyes_gap;
  paddle.right_eye.x = paddle.dimensions.x + half_width + eyes_gap;

  SDL_FPoint left_pupil_direction = sub_vec(ball, paddle.left_eye);
  SDL_FPoint left_pupil_normalized = normalize(left_pupil_direction);
  paddle.left_pupil.x = paddle.left_eye.x + (left_pupil_normalized.x * 10);
  paddle.left_pupil.y = paddle.left_eye.y + (left_pupil_normalized.y * 10);

  SDL_FPoint right_pupil_direction = sub_vec(ball, paddle.right_eye);
  SDL_FPoint right_pupil_normalized = normalize(right_pupil_direction);
  paddle.right_pupil.x = paddle.right_eye.x + (right_pupil_normalized.x * 10);
  paddle.right_pupil.y = paddle.right_eye.y + (right_pupil_normalized.y * 10);

  paddle.mouth.x = paddle.dimensions.x + half_width;

  SDL_FPoint ball_distance = sub_vec(ball, paddle.mouth);
  paddle.is_happy = vec_length(ball_distance) < half_window_height;

  if (current_effects >= (int)game_effects::tween) {
    paddle.dimensions.y =
        eased_progress * (window_height - paddle.dimensions.h - 20);
  }
}

void render_eyes(paddle_entity *paddle) {
  const int radius = 15;
  const int radius_pupil = 5;

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (int x = -radius; x <= radius; x += 1) {
    for (int y = -radius; y <= radius; y += 1) {
      if (x * x + y * y <= radius * radius) {
        SDL_RenderDrawPoint(renderer, paddle->left_eye.x + x,
                            paddle->left_eye.y + y);
        SDL_RenderDrawPoint(renderer, paddle->right_eye.x + x,
                            paddle->right_eye.y + y);
      }
    }
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  for (int x = -radius_pupil; x <= radius_pupil; x += 1) {
    for (int y = -radius_pupil; y <= radius_pupil; y += 1) {
      if (x * x + y * y <= radius_pupil * radius_pupil) {
        SDL_RenderDrawPoint(renderer, paddle->left_pupil.x + x,
                            paddle->left_pupil.y + y);
        SDL_RenderDrawPoint(renderer, paddle->right_pupil.x + x,
                            paddle->right_pupil.y + y);
      }
    }
  }
}

void render_mouth(paddle_entity *paddle) {
  const int r = 15;
  SDL_FPoint happy = {.x = 0, .y = r};
  SDL_FPoint sad = {.x = -r, .y = 0};
  SDL_FPoint current_mood = paddle->is_happy ? happy : sad;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  for (int x = -r; x <= r; x += 1) {
    for (int y = current_mood.x; y <= current_mood.y; y += 1) {
      if (x * x + y * y <= r * r) {
        SDL_RenderDrawPoint(renderer, paddle->mouth.x + x, paddle->mouth.y + y);
      }
    }
  }
}

void render_paddle(paddle_entity *paddle) {
  if (current_effects >= (int)game_effects::color)
    SDL_SetRenderDrawColor(renderer, 255, 161, 0, 255);

  render_rect(&paddle->dimensions);
  if (current_effects >= (int)game_effects::paddle_eyes)
    render_eyes(paddle);

  if (current_effects >= (int)game_effects::paddle_mouth)
    render_mouth(paddle);

  if (current_effects >= (int)game_effects::color)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

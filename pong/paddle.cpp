#pragma once
#include <cstdint>
#include <cstdio>

// #include <bitset>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "window.cpp"

typedef struct paddle_input {
  bool up;
  bool down;
} paddle_input;
// std::bitset<2> paddle_input("00");

typedef struct paddle_keys {
  SDL_Keycode up;
  SDL_Keycode down;
} paddle_keys;

typedef struct paddle_entity {
  SDL_FRect dimensions;
  uint score;
  paddle_input input;
  paddle_keys keys;
} paddle_entity;

enum class paddle_side {
  left,
  right,
};

paddle_entity create_paddle(paddle_side side, paddle_keys keys) {
  const float gap = 20;
  const float width = 20;
  const float height = 100;

  return {
      .dimensions =
          {
              .x = side == paddle_side::left ? gap : WINDOW_WIDTH - width - gap,
              .y = HALF_WINDOW_H - (height / 2),
              .w = width,
              .h = height,
          },
      .score = 0,
      .input =
          {
              .up = false,
              .down = false,
          },
      .keys = keys,
  };
}

void input_paddle(paddle_entity &paddle, SDL_EventType event_type,
                  SDL_Keycode key_pressed) {
#pragma clang diagnostic ignored "-Wswitch"
  switch (event_type) {
  case SDL_KEYDOWN:
    if (key_pressed == paddle.keys.up) {
      paddle.input.up = true;
    }
    if (key_pressed == paddle.keys.down) {
      paddle.input.down = true;
    }
    break;
  case SDL_KEYUP:
    if (key_pressed == paddle.keys.up) {
      paddle.input.up = false;
    }
    if (key_pressed == paddle.keys.down) {
      paddle.input.down = false;
    }
  }
}

void update_paddle(paddle_entity &paddle) {
  const int paddle_speed = 10;

  if (paddle.input.up)
    paddle.dimensions.y = SDL_clamp(paddle.dimensions.y - paddle_speed, 0,
                                    WINDOW_HEIGHT - paddle.dimensions.h);

  if (paddle.input.down)
    paddle.dimensions.y = SDL_clamp(paddle.dimensions.y + paddle_speed, 0,
                                    WINDOW_HEIGHT - paddle.dimensions.h);
}

void render_paddle(SDL_Renderer *renderer, paddle_entity *paddle) {
  SDL_RenderFillRectF(renderer, &paddle->dimensions);
}

void render_score(TTF_Font *font, SDL_Renderer *renderer, paddle_side side,
                  paddle_entity paddle) {
  int padding = side == paddle_side::left ? 4 : 2;
  int direction = side == paddle_side::left ? -1 : 1;

  SDL_Color text_color = {255, 255, 255, 255};

  SDL_Surface *surface = TTF_RenderText_Solid(
      font, std::to_string(paddle.score).c_str(), text_color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect dstrect = {
      .x = HALF_WINDOW_W + (surface->w * padding) * direction,
      .y = 15,
      .w = surface->w * 2,
      .h = surface->h * 2,
  };
  SDL_RenderCopy(renderer, texture, NULL, &dstrect);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}
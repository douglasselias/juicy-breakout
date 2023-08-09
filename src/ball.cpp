#pragma once
#include <cstdint>

#include <SDL2/SDL.h>

#include "window.cpp"

const uint8_t ball_radius = 10;

int64_t ball_base_speed = 6;
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

void draw_circle(SDL_Renderer *renderer, int center_x, int center_y,
                 int radius) noexcept {
  for (int x = -radius; x <= radius; x += 1) {
    for (int y = -radius; y <= radius; y += 1) {
      if (x * x + y * y <= radius * radius) {
        SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
      }
    }
  }
}
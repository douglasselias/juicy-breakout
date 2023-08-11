#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <tuple>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int HALF_WINDOW_W = WINDOW_WIDTH / 2;
const int HALF_WINDOW_H = WINDOW_HEIGHT / 2;

int last_frame_time = 0;

SDL_Renderer *create_window() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
  return SDL_CreateRenderer(window, -1, 0);
}

// void init_font() {}

void cap_framerate() {
  const int fps = 60;
  const float one_second = 1000;
  const float frame_target_time = one_second / fps;

  int time_to_wait = frame_target_time - (SDL_GetTicks64() - last_frame_time);
  if (time_to_wait > 0 && time_to_wait <= frame_target_time) {
    SDL_Delay(time_to_wait);
  }
}

// converted to seconds by dividing by 1000.0f
float get_delta_time() {
  return (SDL_GetTicks64() - last_frame_time) / 1000.0f;
}
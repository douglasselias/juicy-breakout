#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

const int window_width = 1280;
const int window_height = 720;
const int half_window_width = window_width / 2;
const int half_window_height = window_height / 2;

int last_frame_time = 0;

SDL_Renderer *renderer;

SDL_Renderer *create_window() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       window_width, window_height, SDL_WINDOW_BORDERLESS);
  renderer = SDL_CreateRenderer(window, -1, 0);
  return renderer;
}

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

void clear_window() {
  SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
  SDL_RenderClear(renderer);
}

void swap_buffers() { SDL_RenderPresent(renderer); }
#pragma once
#include <SDL2/SDL_keycode.h>
#include <cstdio>
#include <cstdlib>

#include "paddle.cpp"

enum game_state {
  main_menu,
  playing,
  paused,
  game_over,
};
int current_game_state = main_menu;
bool game_is_running = true;

void input_game_state(SDL_EventType event_type, SDL_Keycode key_pressed,
                      paddle_entity &paddle1, paddle_entity &paddle2,
                      SDL_FPoint &ball_position) {
  switch (event_type) {
  case SDL_KEYDOWN:
    switch (key_pressed) {
    case SDLK_ESCAPE:
      game_is_running = false;
      break;
    case SDLK_RETURN:
      switch (current_game_state) {
      case main_menu:
        current_game_state = playing;
        break;
      case playing:
        current_game_state = paused;
        break;
      case game_over:
        paddle1 =
            create_paddle(paddle_side::left, {.up = SDLK_w, .down = SDLK_s});
        paddle2 =
            create_paddle(paddle_side::right, {.up = SDLK_i, .down = SDLK_k});
        ball_position = {
            .x = HALF_WINDOW_W,
            .y = HALF_WINDOW_H,
        };
        current_game_state = playing;
        break;
      }
    }
  }
}

uint max_score = 3;
void update_game_over(paddle_entity paddle) {
  if (paddle.score == max_score)
    current_game_state = game_over;
}
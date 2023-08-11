#pragma once
#include <cstdlib>

enum game_state {
  main_menu,
  controls_menu,
  playing,
  paused,
  game_over,
  _quit_game,
};
int current_game_state = main_menu;
int game_is_running = true;
uint max_score = 1;

void quit_game() { game_is_running = false; }
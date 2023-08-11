#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

#include "src/window.cpp"

enum game_state {
  main_menu,
  playing,
  paused,
  game_over,
};
int current_game_state = main_menu;
bool game_is_running = true;

int main(void) {
  SDL_Renderer *renderer = create_window();
  while (game_is_running) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        game_is_running = false;
      }
    }

    clear_window();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, 2, 0, 300 / 2, 400);
    swap_buffers();
  }
}
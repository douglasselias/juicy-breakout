#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>

#include "src/ball.cpp"
#include "src/blocks.cpp"
#include "src/paddle.cpp"
#include "src/window.cpp"

enum game_state {
  main_menu,
};
int current_game_state = main_menu;
bool game_is_running = true;

int main(void) {
  SDL_Renderer *renderer = create_window();
  paddle_entity paddle = create_paddle({.left = SDLK_a, .right = SDLK_d});
  blocks blocks = create_blocks();
  SDL_FRect ball = create_ball();

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

    input_paddle(paddle, (SDL_EventType)event.type, event.key.keysym.sym);

    cap_framerate();
    last_frame_time = SDL_GetTicks64();

    update_paddle(paddle);
    update_ball(ball);

    ball_paddle_collision(ball, paddle);
    ball_blocks_collision(ball, blocks);

    clear_window();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    render_paddle(&paddle);
    render_ball(ball);
    render_blocks(blocks);
    swap_buffers();
  }
}
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_timer.h>

#include "src/ball.cpp"
#include "src/blocks.cpp"
#include "src/effects.cpp"
#include "src/paddle.cpp"
#include "src/window.cpp"

bool game_is_running = true;

int main(void) {
  SDL_Renderer *renderer = create_window();
  paddle_entity paddle = create_paddle({.left = SDLK_a, .right = SDLK_d});
  blocks blocks = create_blocks();
  SDL_FRect ball = create_ball();

  Uint32 startTime = SDL_GetTicks64();
  float progress = 0;
  float eased_progress = 0;

  while (game_is_running) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        game_is_running = false;
        break;
      case SDLK_b:
        if (current_effects > -1)
          current_effects -= 1;
        break;
      case SDLK_n:
        if (current_effects < 40) {
          current_effects += 1;
          if (current_effects >= (int)game_effects::tween) {
            startTime = SDL_GetTicks64();
            paddle.dimensions.y = -80;

            for (block &b : blocks)
              b.dimensions.y = b.original_dimensions.y * 10;
          }
        }
        break;
      }
    }

    if (current_effects >= (int)game_effects::tween) {
      Uint32 currentTime = SDL_GetTicks64();
      progress = static_cast<float>(currentTime - startTime) / duration;
      if (progress > 1.0f)
        progress = 1.0f;

      eased_progress = ease_in_out_quart(progress);
    }

    input_paddle(paddle, (SDL_EventType)event.type, event.key.keysym.sym);

    cap_framerate();
    last_frame_time = SDL_GetTicks64();

    update_paddle(paddle, eased_progress);
    update_ball(ball);
    update_blocks(blocks, eased_progress);

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
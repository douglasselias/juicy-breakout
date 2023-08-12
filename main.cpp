#include <SDL2/SDL.h>

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <cstdio>
#include <ctime>

#include "src/ball.cpp"
#include "src/blocks.cpp"
#include "src/effects.cpp"
#include "src/paddle.cpp"
#include "src/particles.cpp"
#include "src/window.cpp"

bool game_is_running = true;
bool is_playing_bgm = false;
int bgm_channel = -1;

int main(void) {
  SDL_Renderer *renderer = create_window();
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  init_audio();
  Mix_Chunk *bgm = load_audio("bgm.ogg");
  paddle_entity paddle = create_paddle();
  blocks blocks = create_blocks();
  SDL_FRect ball = create_ball();

  Uint32 start_time = SDL_GetTicks64();
  float progress = 0;
  float eased_progress = 0;
  // Duration of the tween in milliseconds
  const Uint32 duration = 1200;

  int shakeMagnitude = 5;  // Adjust the magnitude of the shake
  int shakeDuration = 200; // Duration of each shake in milliseconds

  while (game_is_running) {
    SDL_Event event;
    SDL_PollEvent(&event);

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        game_is_running = false;
        break;
      case SDLK_a:
        if (current_effects == (int)game_effects::hit_stop)
          current_effects = -1;
        else
          current_effects = (int)game_effects::hit_stop;
        break;
      case SDLK_b:
        if (current_effects > -1)
          current_effects -= 1;

        if (current_effects < (int)game_effects::bgm) {
          is_playing_bgm = false;
          Mix_HaltChannel(bgm_channel);
        }
        break;
      case SDLK_n:
        if (current_effects < 40) {
          current_effects += 1;
          if (current_effects == (int)game_effects::tween) {
            start_time = SDL_GetTicks64();
            paddle.dimensions.y = -800;

            for (block &b : blocks)
              b.dimensions.y = b.original_dimensions.y * 10;
          }
        }
        break;
      }
    }

    if (current_effects == (int)game_effects::tween) {
      Uint32 current_time = SDL_GetTicks64();
      progress = static_cast<float>(current_time - start_time) / duration;
      if (progress > 1.0f)
        progress = 1.0f;

      eased_progress = ease_in_out_back(progress);
    }

    if (current_effects >= (int)game_effects::bgm) {
      if (!is_playing_bgm)
        bgm_channel = play_audio(bgm);
      is_playing_bgm = true;
    }

    cap_framerate();
    float delta_time = get_delta_time();
    last_frame_time = SDL_GetTicks64();

    if (current_effects >= (int)game_effects::ball_smoke_particles)
      update_particles(particles);

    update_paddle(paddle, eased_progress, mouse_x, {.x = ball.x, .y = ball.y});
    update_ball(ball, delta_time);
    update_blocks(blocks, eased_progress);

    ball_paddle_collision(ball, paddle);
    ball_blocks_collision(ball, blocks);

    if (shake) {
      float elapsed = SDL_GetTicks64() - shake_ticks;
      if (elapsed < shakeDuration) {
        int offsetX = (std::rand() % shakeMagnitude * 2) - shakeMagnitude;
        int offsetY = (std::rand() % shakeMagnitude * 2) - shakeMagnitude;

        SDL_RenderSetViewport(renderer, NULL);
        SDL_Rect shakeViewport = {offsetX, offsetY, window_width,
                                  window_height};
        SDL_RenderSetViewport(renderer, &shakeViewport);
      } else {
        SDL_RenderSetViewport(renderer, NULL);
        shake = false;
      }
    }

    if (slow_motion) {
      float elapsed = SDL_GetTicks64() - slow_motion_ticks;
      if (elapsed > slow_motion_duration) {
        restoreSlowMotion();
        slow_motion = false;
      }
    }

    clear_window();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    render_paddle(&paddle);
    render_ball(ball);
    render_blocks(blocks);

    if (current_effects >= (int)game_effects::ball_smoke_particles)
      render_particles(renderer, particles);

    swap_buffers();
  }
}
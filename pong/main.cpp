#include <cmath>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
// #include <box2d/box2d.h>

#include "ball.cpp"
#include "font.cpp"
#include "game_state.cpp"
#include "paddle.cpp"
#include "window.cpp"

// b2Vec2 gravity(0.0f, 0.0f);
// b2World world(gravity);

bool ball_paddle_collision(SDL_FPoint ball_center, int ball_radius,
                           SDL_FRect paddle) {
  float ball_top_y = ball_center.y - ball_radius;
  float ball_bottom_y = ball_center.y + ball_radius;

  float paddle_top_y = paddle.y;
  float paddle_bottom_y = paddle.y + paddle.h;

  float lowest_y = SDL_min(SDL_min(ball_top_y, ball_bottom_y),
                           SDL_min(paddle_top_y, paddle_bottom_y));
  float highest_y = SDL_max(SDL_max(ball_top_y, ball_bottom_y),
                            SDL_max(paddle_top_y, paddle_bottom_y));

  float total_size_y = highest_y - lowest_y;

  float ball_radii = ball_radius * 2;

  float minimum_size_for_collision_in_y_axis = ball_radii + paddle.h;

  if (total_size_y > minimum_size_for_collision_in_y_axis) {
    // if greater then it has a gap
    return false;
  }

  float ball_top_x = ball_center.x - ball_radius;
  float ball_bottom_x = ball_center.x + ball_radius;

  float paddle_top_x = paddle.x;
  float paddle_bottom_x = paddle.x + paddle.w;

  float lowest_x = SDL_min(SDL_min(ball_top_x, ball_bottom_x),
                           SDL_min(paddle_top_x, paddle_bottom_x));
  float highest_x = SDL_max(SDL_max(ball_top_x, ball_bottom_x),
                            SDL_max(paddle_top_x, paddle_bottom_x));

  float total_size_x = highest_x - lowest_x;
  float minimum_size_for_collision_in_x_axis = ball_radii + paddle.w;

  if (total_size_x > minimum_size_for_collision_in_x_axis) {
    // if greater then it has a gap
    return false;
  }

  return true;
}

paddle_entity paddle1 =
    create_paddle(paddle_side::left, {.up = SDLK_w, .down = SDLK_s});
paddle_entity paddle2 =
    create_paddle(paddle_side::right, {.up = SDLK_i, .down = SDLK_k});

int main(void) {
  SDL_Renderer *renderer = create_window();
  TTF_Font *font = load_font();

  text_render_data game_over_text = create_game_over_text(renderer);
  text_render_data start_game_text = create_start_game_text(renderer);

  while (game_is_running) {
    // --------- PROCESS INPUT --------- //
    SDL_Event event;
    SDL_PollEvent(&event);

    input_game_state((SDL_EventType)event.type, event.key.keysym.sym, paddle1,
                     paddle2, ball_position);

    switch (current_effects) {
    case playing:
      input_paddle(paddle1, (SDL_EventType)event.type, event.key.keysym.sym);
      input_paddle(paddle2, (SDL_EventType)event.type, event.key.keysym.sym);
      break;
    }

    // --------- UPDATE --------- //
    cap_framerate();
    // float delta_time = get_delta_time();
    last_frame_time = SDL_GetTicks64();

    switch (current_effects) {
    case playing:
      update_paddle(paddle1);
      update_paddle(paddle2);

      if (ball_paddle_collision(ball_position, ball_radius,
                                paddle1.dimensions)) {
        invert_x = invert_x == 1 ? -1 : 1;
        ball_speed_x += ball_speed_increase_x;
        ball_speed_y += ball_speed_increase_y;

        SDL_FRect player1_section_top = {
            .x = paddle1.dimensions.x,
            .y = paddle1.dimensions.y,
            .w = paddle1.dimensions.w,
            .h = paddle1.dimensions.h / 4,
        };

        if (ball_paddle_collision(ball_position, ball_radius,
                                  player1_section_top)) {
          ball_speed_y -= ball_speed_increase_y * 4;
        }

        SDL_FRect player1_section_bottom = {
            .x = paddle1.dimensions.x,
            .y = paddle1.dimensions.y + (paddle1.dimensions.h / 4) * 3,
            .w = paddle1.dimensions.w,
            .h = paddle1.dimensions.h / 4,
        };

        if (ball_paddle_collision(ball_position, ball_radius,
                                  player1_section_bottom)) {
          ball_speed_y += ball_speed_increase_y * 4;
        }
      }

      if (ball_paddle_collision(ball_position, ball_radius,
                                paddle2.dimensions)) {
        invert_x = invert_x == 1 ? -1 : 1;
        ball_speed_x += ball_speed_increase_x;
        ball_speed_y += ball_speed_increase_y;

        SDL_FRect player2_section_top = {
            .x = paddle2.dimensions.x,
            .y = paddle2.dimensions.y,
            .w = paddle2.dimensions.w,
            .h = paddle2.dimensions.h / 4,
        };

        if (ball_paddle_collision(ball_position, ball_radius,
                                  player2_section_top)) {
          ball_speed_y -= ball_speed_increase_y * 4;
        }

        SDL_FRect player2_section_bottom = {
            .x = paddle2.dimensions.x,
            .y = paddle2.dimensions.y + (paddle2.dimensions.h / 4) * 3,
            .w = paddle2.dimensions.w,
            .h = paddle2.dimensions.h / 4,
        };

        if (ball_paddle_collision(ball_position, ball_radius,
                                  player2_section_bottom)) {
          ball_speed_y += ball_speed_increase_y * 4;
        }
      }

      update_ball(paddle1, paddle2);

      update_game_over(paddle1);
      update_game_over(paddle2);
      break;
    }

    // --------- RENDERING --------- //
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    switch (current_effects) {
    case game_over:
      SDL_RenderCopy(renderer, game_over_text.texture, NULL,
                     &game_over_text.dstrect);
      break;
    case main_menu:
      SDL_RenderCopy(renderer, start_game_text.texture, NULL,
                     &start_game_text.dstrect);
      break;
    case playing:
      SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2,
                         WINDOW_HEIGHT);
      render_score(font, renderer, paddle_side::left, paddle1);
      render_score(font, renderer, paddle_side::right, paddle2);
      render_paddle(renderer, &paddle1);
      render_paddle(renderer, &paddle2);
      render_ball(renderer, ball_position);
      break;
    }

    SDL_RenderPresent(renderer); // double buffer swap
  }

  return EXIT_SUCCESS;
}
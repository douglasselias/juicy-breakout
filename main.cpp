#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_timer.h>
#include <cstddef>
#include <cstdint>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int HALF_WINDOW_W = WINDOW_WIDTH / 2;
const int HALF_WINDOW_H = WINDOW_HEIGHT / 2;

const int FPS = 60;
const int FRAME_TARGET_TIME = (1000 / FPS);
int last_frame_time = 0;

typedef struct paddle_size {
  float width;
  float height;
} paddle_size;

paddle_size player_size = {
    .width = 20,
    .height = 100,
};

typedef struct player_input {
  uint8_t up;
  uint8_t down;
} player_input;

float degree_to_radian(float degree) { return degree * (M_PI / 180); }

void draw_circle(SDL_Renderer *renderer, int center_x, int center_y,
                 int radius) {
  for (int x = -radius; x <= radius; x++) {
    for (int y = -radius; y <= radius; y++) {
      if (x * x + y * y <= radius * radius) {
        SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
      }
    }
  }
}

float clamp_player_position(float position) {
  return SDL_clamp(position, 0, WINDOW_HEIGHT - player_size.height);
}

int main(int argc, char *args[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL\n");
    return EXIT_FAILURE;
  }

  SDL_Window *window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);

  if (window == NULL) {
    fprintf(stderr, "Error creating SDL window\n");
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  if (renderer == NULL) {
    fprintf(stderr, "Error creating SDL renderer\n");
    return EXIT_FAILURE;
  }

  // setup();
  int game_is_running = true;

  float initial_y_position = HALF_WINDOW_H - (player_size.height / 2);

  SDL_FPoint player1_position = {
      .x = 10,
      .y = initial_y_position,
  };

  SDL_FPoint player2_position = {
      .x = WINDOW_WIDTH - player_size.width - 10,
      .y = initial_y_position,
  };

  player_input player1_input = {
      .up = 0,
      .down = 0,
  };

  player_input player2_input = {
      .up = 0,
      .down = 0,
  };

  int paddle_speed = 10;

  SDL_FPoint ball_position = {
      .x = HALF_WINDOW_W,
      .y = HALF_WINDOW_H,
  };

  int ball_radius = 10;
  int ball_speed = 6;

  int invert_x = false;
  int invert_y = false;

  while (game_is_running) {
    // --------- PROCESS INPUT --------- //
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
      game_is_running = false;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        game_is_running = false;
        break;

      case SDLK_w:
        player1_input.up = 1;
        break;
      case SDLK_s:
        player1_input.down = 1;
        break;

      case SDLK_i:
        player2_input.up = 1;
        break;
      case SDLK_k:
        player2_input.down = 1;
        break;
      }
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_w:
        player1_input.up = 0;
        break;
      case SDLK_s:
        player1_input.down = 0;
        break;

      case SDLK_i:
        player2_input.up = 0;
        break;
      case SDLK_k:
        player2_input.down = 0;
        break;
      }
      break;
    }

    // --------- UPDATE --------- //
    // cap framerate to 60 fps
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
      SDL_Delay(time_to_wait);
    }
    float delta_time = (SDL_GetTicks() - last_frame_time) /
                       1000.0f; // converted to seconds by dividing by 1000.0f
    last_frame_time = SDL_GetTicks();

    if (player1_input.up)
      player1_position.y =
          clamp_player_position(player1_position.y - paddle_speed);
    if (player1_input.down)
      player1_position.y =
          clamp_player_position(player1_position.y + paddle_speed);
    if (player2_input.up)
      player2_position.y =
          clamp_player_position(player2_position.y - paddle_speed);
    if (player2_input.down)
      player2_position.y =
          clamp_player_position(player2_position.y + paddle_speed);

    SDL_FRect player1_rect = {
        .x = player1_position.x,
        .y = player1_position.y,
        .w = player_size.width,
        .h = player_size.height,
    };

    SDL_FRect player2_rect = {
        .x = player2_position.x,
        .y = player2_position.y,
        .w = player_size.width,
        .h = player_size.height,
    };

    if (SDL_PointInFRect(&ball_position, &player1_rect))
      invert_x = !invert_x;

    if (SDL_PointInFRect(&ball_position, &player2_rect))
      invert_x = !invert_x;

    if (ball_position.x + ball_radius + ball_speed > WINDOW_WIDTH ||
        ball_position.x - ball_radius + ball_speed < 0)
      invert_x = !invert_x;

    if (ball_position.y + ball_radius + ball_speed > WINDOW_HEIGHT ||
        ball_position.y - ball_radius + ball_speed < 0)
      invert_y = !invert_y;

    ball_position.x += ball_speed * (invert_x ? -1 : 1);
    ball_position.y += ball_speed * (invert_y ? -1 : 1);

    // --------- RENDERING --------- //
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    float radius = (float)WINDOW_HEIGHT / 2;
    draw_circle(renderer, ball_position.x, ball_position.y, ball_radius);
    SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2,
                       WINDOW_HEIGHT);

    SDL_FRect player1 = {
        .x = player1_position.x,
        .y = player1_position.y,
        .w = player_size.width,
        .h = player_size.height,
    };

    SDL_FRect player2 = {
        .x = player2_position.x,
        .y = player2_position.y,
        .w = player_size.width,
        .h = player_size.height,
    };

    SDL_RenderFillRectF(renderer, &player1);
    SDL_RenderFillRectF(renderer, &player2);

    SDL_RenderPresent(renderer); // double buffer swap
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
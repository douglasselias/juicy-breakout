#include <SDL2/SDL_log.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
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
    .width = 10,
    .height = 50,
};

typedef struct player_input {
  int up;
  int down;
} player_input;

float degree_to_radian(float degree) { return degree * (M_PI / 180); }

void draw_circle(SDL_Renderer *renderer, int center_x, int center_y,
                 int radius) {
  int precision = 360 * 5;
  for (int i = 0; i <= precision; i++) {
    float radian = degree_to_radian(i);
    int x = center_x + (SDL_cos(radian) * radius);
    int y = center_y + (SDL_sin(radian) * radius);

    SDL_RenderDrawPoint(renderer, x, y);
  }
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

  int ball_radius = 5;

  while (game_is_running) {
    // process_input();
    SDL_Event event;
    SDL_PollEvent(&event);

    // player1_input = {0, 0};
    // player2_input = {0, 0};

    switch (event.type) {
    case SDL_QUIT:
      game_is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        game_is_running = false;

      player1_input.up = event.key.keysym.sym == SDLK_w;
      player1_input.down = event.key.keysym.sym == SDLK_s;

      player2_input.up = event.key.keysym.sym == SDLK_i;
      player2_input.down = event.key.keysym.sym == SDLK_k;
      break;
    case SDL_KEYUP:
      player1_input.up = event.key.keysym.sym != SDLK_w;
      player1_input.down = event.key.keysym.sym == SDLK_s;

      player2_input.up = event.key.keysym.sym != SDLK_i;
      player2_input.down = event.key.keysym.sym != SDLK_k;
      break;
    }

    // update();
    // cap framerate to 60 fps
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
      SDL_Delay(time_to_wait);
    }
    float delta_time = (SDL_GetTicks() - last_frame_time) /
                       1000.0f; // converted to seconds by dividing by 1000.0f
    last_frame_time = SDL_GetTicks();

    if (player1_input.up)
      player1_position.y -= paddle_speed;
    if (player1_input.down)
      player1_position.y += paddle_speed;
    if (player2_input.up)
      player2_position.y += paddle_speed;
    if (player2_input.down)
      player2_position.y -= paddle_speed;

    ball_position.x += 2 * delta_time;
    ball_position.y += 2 * delta_time;
    // SDL_PointInFRect(const SDL_FPoint *p, const SDL_FRect *r);
    // SDL_PointInRect(const SDL_Point *p, const SDL_Rect *r);

    // render();
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
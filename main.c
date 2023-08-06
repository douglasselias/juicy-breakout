#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int FPS = 75;
const int FRAME_TARGET_TIME = (1000 / FPS);

struct ball {
  float x;
  float y;
  float width;
  float height;
} ball;

int last_frame_time = 0;

float degree_to_radian(float degree) {
  float pi = 3.1415;
  return degree * (pi / 180);
}

void plot_pixel(SDL_Renderer *renderer, int x, int y) {
  int size = 1;
  SDL_Rect pixel = {
      .x = x,
      .y = y,
      .w = size,
      .h = size,
  };
  SDL_RenderFillRect(renderer, &pixel);
}

void draw_circle_outline(SDL_Renderer *renderer, int center_x, int center_y, int radius) {
  int precision = 360;
  for (int i = 0; i <= precision; i++) {
    float radian = degree_to_radian(i);
    int x = center_x + (cos(radian) * radius);
    int y = center_y + (sin(radian) * radius);

    plot_pixel(renderer, x, y);
  }
}

int main(int argc, char *args[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL\n");
    return EXIT_FAILURE;
  }
  // atexit(SDL_Quit);

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
  // ball.x = (float)WINDOW_WIDTH / 2;
  // ball.y = (float)WINDOW_HEIGHT / 2;
  // ball.width = 1;
  // ball.height = 1;

  while (game_is_running) {
    // process_input();
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
      game_is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        game_is_running = false;
      break;
    }

    // update();
    // int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() -
    // last_frame_time); if (time_to_wait > 0 && time_to_wait <=
    // FRAME_TARGET_TIME) {
    //   SDL_Delay(time_to_wait);
    // }
    // float delta_time = (SDL_GetTicks() - last_frame_time) /
    //                    1000.0f; // converted to seconds by dividing by
    //                    1000.0f
    last_frame_time = SDL_GetTicks();

    // ball.x += 70 * delta_time;
    // ball.y += 50 * delta_time;

    // render();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    draw_circle_outline(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
                  WINDOW_HEIGHT / 2);

    SDL_RenderPresent(renderer); // double buffer swap
  }

  // cleanup();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
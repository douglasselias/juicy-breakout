#include <cstddef>
#include <cstdint>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
// const int WINDOW_WIDTH = 2560;
// const int WINDOW_HEIGHT = 1080;

const int FPS = 60;
const int FRAME_TARGET_TIME = (1000 / FPS);

struct ball {
  float x;
  float y;
  float width;
  float height;
} ball;

int last_frame_time = 0;

void get_sdl_version() {
  SDL_version compiled;
  SDL_version linked;

  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  SDL_Log("Compiled SDL v%u.%u.%u\n", compiled.major, compiled.minor,
          compiled.patch);
  SDL_Log("Linked SDL v%u.%u.%u\n", linked.major, linked.minor, linked.patch);
  SDL_Log("SDL compile version: %d.%d.%d\n", SDL_MAJOR_VERSION,
          SDL_MINOR_VERSION, SDL_PATCHLEVEL);
}

int display_mode_logs() {
  static int display_in_use = 0; /* Only using first display */

  int i, display_mode_count;
  SDL_DisplayMode mode;
  Uint32 f;

  SDL_Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());

  display_mode_count = SDL_GetNumDisplayModes(display_in_use);
  if (display_mode_count < 1) {
    SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
    return 1;
  }
  SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);

  for (i = 0; i < display_mode_count; ++i) {
    if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
      SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
      return 1;
    }
    // f = mode.format;

    SDL_Log("%i x %i, > %dHz", mode.w, mode.h, mode.refresh_rate);
  }
  return 0;
}

float degree_to_radian(float degree) {
  // float pi = 3.1415;
  return degree * (M_PI / 180);
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
  // SDL_RenderDrawRect(renderer, &pixel);
}

int cycle = 0;
float rotation_speed = degree_to_radian(1);
void draw_circle_outline(SDL_Renderer *renderer, int center_x, int center_y,
                         int radius) {
  int precision = 360 * 5;
  for (int i = 0; i <= precision; i++) {
    float radian = degree_to_radian(i);
    int x = center_x + ((SDL_cos(radian)+rotation_speed) * radius);
    int y = center_y + ((SDL_sin(radian)+rotation_speed) * radius);

    // plot_pixel(renderer, x, y);
    // SDL_RenderDrawPoint(renderer, x, y);
    // SDL_SetRenderDrawColor(renderer, i % 255, (i + 85) % 255,
    //                        (i + 85 + 85) % 255, 255);
    SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, x, y);

    if (i == 360)
      rotation_speed += rotation_speed;
  }
}

int main(int argc, char *args[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL\n");
    return EXIT_FAILURE;
  }
  // atexit(SDL_Quit);

  SDL_Window *window = SDL_CreateWindow(
      NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
      WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL);

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
  // display_mode_logs();
  get_sdl_version();
  // SDL_Log("Is running on %d", SDL_HasSSE3());
  // SDL_ShowSimpleMessageBox(0, "Hello World!", "Play the game", window);

  SDL_Vertex vertex_1 = {{10, 10}, {255, 30, 255, 255}, {1, 1}};
  SDL_Vertex vertex_2 = {{200, 10}, {0, 130, 255, 255}, {1, 1}};
  SDL_Vertex vertex_3 = {{10, 200}, {0, 120, 255, 255}, {1, 1}};

  SDL_Vertex vertices[] = {vertex_1, vertex_2, vertex_3};

  SDL_Vertex vertex_1_l = {
      {WINDOW_WIDTH - 10, 10}, {255, 30, 255, 255}, {1, 1}};
  SDL_Vertex vertex_2_l = {
      {WINDOW_WIDTH - 200, 10}, {0, 130, 120, 255}, {1, 1}};
  SDL_Vertex vertex_3_l = {
      {WINDOW_WIDTH - 10, 200}, {0, 120, 255, 255}, {1, 1}};

  SDL_Vertex vertices2[] = {vertex_1_l, vertex_2_l, vertex_3_l};

  while (game_is_running) {
    // process_input();
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
      game_is_running = false;
      break;
    case SDL_APP_DIDENTERBACKGROUND:
      SDL_Log("IN Background! not working");
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        game_is_running = false;
      if (event.key.keysym.sym == SDLK_a) {
      }
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

    // ball.x += 70 * delta_time;
    // ball.y += 50 * delta_time;
    // SDL_PointInFRect(const SDL_FPoint *p, const SDL_FRect *r);
    // SDL_PointInRect(const SDL_Point *p, const SDL_Rect *r);

    // render();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
    SDL_RenderGeometry(renderer, NULL, vertices2, 3, NULL, 0);

    float radius = (float)WINDOW_HEIGHT / 2;
    draw_circle_outline(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, radius);
    SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
                       ((float)WINDOW_WIDTH / 2) + radius, radius);

    SDL_RenderPresent(renderer); // double buffer swap
  }

  // cleanup();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
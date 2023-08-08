#include <cmath>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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

void draw_circle(SDL_Renderer *renderer, int center_x, int center_y,
                 int radius) {
  for (int x = -radius; x <= radius; x += 1) {
    for (int y = -radius; y <= radius; y += 1) {
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

  if (TTF_Init() == -1) {
    fprintf(stderr, "SDL_ttf initialization failed: %s", TTF_GetError());
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  if (renderer == NULL) {
    fprintf(stderr, "Error creating SDL renderer\n");
    return EXIT_FAILURE;
  }

  TTF_Font *font = TTF_OpenFont("PressStart2P.ttf", 16);
  if (font == NULL) {
    fprintf(stderr, "Font loading failed: %s", TTF_GetError());
    return EXIT_FAILURE;
  }

  SDL_Color textColor = {255, 255, 255, 255};
  SDL_Surface *game_over_surface =
      TTF_RenderText_Solid(font, "Game over", textColor);
  if (game_over_surface == NULL) {
    fprintf(stderr, "Text rendering failed: %s", TTF_GetError());
    return EXIT_FAILURE;
  }

  SDL_Texture *game_over_texture =
      SDL_CreateTextureFromSurface(renderer, game_over_surface);
  if (game_over_texture == NULL) {
    fprintf(stderr, "Texture creation failed: %s", SDL_GetError());
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

  int player1_score = 0;
  int player2_score = 0;

  enum game_state {
    menu,
    playing,
    game_over,
  };

  int current_game_state = menu;

  int max_score = 1;

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

    bool hit_left_wall = ball_position.x - ball_radius + ball_speed < 0;
    bool hit_right_wall =
        ball_position.x + ball_radius + ball_speed > WINDOW_WIDTH;

    if (hit_left_wall)
      player2_score += 1;

    if (hit_right_wall)
      player1_score += 1;

    if (player1_score == max_score)
      current_game_state = game_over;

    if (player2_score == max_score)
      current_game_state = game_over;

    if (hit_right_wall || hit_left_wall)
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

    SDL_Surface *text_surf = TTF_RenderText_Solid(
        font, std::to_string(player1_score).c_str(), textColor);
    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(renderer, text_surf);

    SDL_Rect dest = {};
    dest.x = HALF_WINDOW_W - text_surf->w * 4;
    dest.y = 15;
    dest.w = text_surf->w * 2;
    dest.h = text_surf->h * 2;
    SDL_RenderCopy(renderer, textTexture, NULL, &dest);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(text_surf);

    // player 2 score
    SDL_Surface *text_surf2 = TTF_RenderText_Solid(
        font, std::to_string(player2_score).c_str(), textColor);
    SDL_Texture *textTexture2 =
        SDL_CreateTextureFromSurface(renderer, text_surf2);

    SDL_Rect dest2 = {};
    dest2.x = HALF_WINDOW_W + text_surf2->w * 2;
    dest2.y = 15;
    dest2.w = text_surf2->w * 2;
    dest2.h = text_surf2->h * 2;
    SDL_RenderCopy(renderer, textTexture2, NULL, &dest2);

    SDL_DestroyTexture(textTexture2);
    SDL_FreeSurface(text_surf2);
    // player 2 score

    if (current_game_state == game_over) {
      SDL_Rect game_over_dstrect = {};
      game_over_dstrect.x = HALF_WINDOW_W - game_over_surface->w;
      game_over_dstrect.y = HALF_WINDOW_H - game_over_surface->h;
      game_over_dstrect.w = game_over_surface->w * 2;
      game_over_dstrect.h = game_over_surface->h * 2;
      SDL_RenderCopy(renderer, game_over_texture, NULL, &game_over_dstrect);
    }

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

  SDL_FreeSurface(game_over_surface);
  SDL_DestroyTexture(game_over_texture);
  TTF_CloseFont(font);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "window.cpp"

TTF_Font *font = nullptr;

TTF_Font *load_font() {
  TTF_Init();
  font = TTF_OpenFont("PressStart2P.ttf", 16);
  return font;
}

SDL_Color text_color = {255, 255, 255, 255};

void render_text(SDL_Renderer *renderer, SDL_FPoint position,
                 std::string text) {
  SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), text_color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect dstrect = {
      .x = (int)position.x,
      .y = (int)position.y,
      .w = surface->w * 2,
      .h = surface->h * 2,
  };
  SDL_RenderCopy(renderer, texture, NULL, &dstrect);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}

typedef struct text_render_data {
  SDL_Texture *texture;
  SDL_Rect dstrect;
} text_render_data;

text_render_data create_game_over_text(SDL_Renderer *renderer) {
  SDL_Color text_color = {255, 255, 255, 255};
  SDL_Surface *game_over_surface =
      TTF_RenderText_Solid(font, "Game over", text_color);
  SDL_Texture *game_over_texture =
      SDL_CreateTextureFromSurface(renderer, game_over_surface);

  SDL_Rect game_over_dstrect = {
      .x = HALF_WINDOW_W - game_over_surface->w,
      .y = HALF_WINDOW_H - game_over_surface->h,
      .w = game_over_surface->w * 2,
      .h = game_over_surface->h * 2,
  };

  return {
      .texture = game_over_texture,
      .dstrect = game_over_dstrect,
  };
}

text_render_data create_start_game_text(SDL_Renderer *renderer) {
  SDL_Color text_color = {255, 255, 255, 255};
  SDL_Surface *game_over_surface =
      TTF_RenderText_Solid(font, "Press start", text_color);
  SDL_Texture *game_over_texture =
      SDL_CreateTextureFromSurface(renderer, game_over_surface);

  SDL_Rect game_over_dstrect = {
      .x = HALF_WINDOW_W - game_over_surface->w,
      .y = HALF_WINDOW_H - game_over_surface->h,
      .w = game_over_surface->w * 2,
      .h = game_over_surface->h * 2,
  };

  return {
      .texture = game_over_texture,
      .dstrect = game_over_dstrect,
  };
}

// void render_game_over(SDL_Renderer* renderer) {
//       SDL_RenderCopy(renderer, game_over_render_data.game_over_texture, NULL,
//                      &game_over_render_data.game_over_dstrect);
// }
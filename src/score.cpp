#pragma once

#include <SDL2/SDL_pixels.h>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "paddle.cpp"
#include "window.cpp"

void draw_score(TTF_Font *font, SDL_Renderer *renderer, SDL_Color text_color, bool is_player_one) {
  int score = is_player_one ? player1_score : player2_score;
  int padding = is_player_one ? 4 : 2;
  int direction = is_player_one ? -1 : 1;

  SDL_Surface *text_surf2 = TTF_RenderText_Solid(
      font, std::to_string(score).c_str(), text_color);
  SDL_Texture *textTexture2 =
      SDL_CreateTextureFromSurface(renderer, text_surf2);

  SDL_Rect dest2 = {
      .x = HALF_WINDOW_W + (text_surf2->w * padding) * direction,
      .y = 15,
      .w = text_surf2->w * 2,
      .h = text_surf2->h * 2,
  };
  SDL_RenderCopy(renderer, textTexture2, NULL, &dest2);

  SDL_DestroyTexture(textTexture2);
  SDL_FreeSurface(text_surf2);
}
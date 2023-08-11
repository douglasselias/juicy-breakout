#pragma once
#include <vector>

#include <SDL2/SDL.h>

#include "effects.cpp"
#include "window.cpp"

typedef struct block {
  SDL_FRect dimensions;
  bool operator==(const block &other) const {
    return dimensions.x == other.dimensions.x &&
           dimensions.y == other.dimensions.y &&
           dimensions.w == other.dimensions.w &&
           dimensions.h == other.dimensions.h;
  }
} block;

using blocks = std::vector<block>;

blocks create_blocks() {
  const int total_rows = 8;
  const int total_columns = 10;
  const float width = 100;
  const float height = 30;
  const float top_gap = 20;
  const float left_gap = 10;
  const float between_gap = 10;

  const int total_grid_width =
      left_gap + (width * total_columns) + (between_gap * total_columns);
  const int initial_left_gap = (window_width - total_grid_width) / 2;

  blocks blocks = {};

  for (int row = 0; row < total_rows; row += 1) {
    for (int column = 0; column < total_columns; column += 1) {
      block b = {
          .dimensions =
              {
                  .x = initial_left_gap + (width * column) +
                       (between_gap * column),
                  .y = top_gap + (height * row) + (between_gap * row),
                  .w = width,
                  .h = height,
              },
      };
      blocks.push_back(b);
    }
  }
  return blocks;
}

void render_blocks(blocks blocks) {
  if (current_effects >= (int)game_effects::color)
    SDL_SetRenderDrawColor(renderer, 230, 41, 55, 255);

  for (block b : blocks)
    render_rect(&b.dimensions);

  if (current_effects >= (int)game_effects::color)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}
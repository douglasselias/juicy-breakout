#include <cstdint>

struct paddle_size {
  float width;
  float height;
};
using paddle_size = paddle_size;

paddle_size player_size = {
    .width = 20,
    .height = 100,
};

typedef struct player_input {
  uint8_t up;
  uint8_t down;
} player_input;
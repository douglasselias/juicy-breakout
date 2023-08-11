flags := "-std=c++14 -Wall -Wextra -Werror -fno-exceptions -fno-rtti -Wno-unused-parameter -Wno-unused-variable -Wno-unused-const-variable -g3 -O0 -lSDL2 -lSDL2_ttf"
# -fsanitize=xxx

just:
  mkdir -p build
  zig c++ {{flags}} main.cpp -o build/main_v0.0.0
  build/main_v0.0.0
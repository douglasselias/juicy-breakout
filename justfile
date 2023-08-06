just:
  # rm -rf build
  mkdir -p build
  zig cc -std=c99 main.c -o build/main_v0.0.0 -lSDL2
  build/main_v0.0.0

cpp:
  mkdir -p build
  clang++ main.cpp -std=c++14 -Isrc -Os -Wall -Werror -o build/main-v0.0.0
  ./build/main-v0.0.0

just:
  mkdir -p build
  zig cc main.c -o build/main -lSDL2

run:
  ./build/main

cpp:
  mkdir -p build
  clang++ main.cpp -std=c++17 -Isrc -Os -Wall -Werror -lraylib -o build/main-v0.0.0
  ./build/main-v0.0.0

c:
  mkdir -p build
  clang main.c -std=c99 -Isrc -Os -Wall -Werror -lSDL2 -lm -o build/main-c-v0.0.0
  ./build/main-c-v0.0.0
build:
  mkdir -p build
  clang++ -std=c++11 -Os -Wall -Werror -o build/juicy-breakout main.cpp -lraylib

run:
  ./build/juicy-breakout

build-run:
  just build
  just run
cxx := "clang++"
flags := "-std=c++17 -Os -Wall -Werror -lraylib -o"
version := "0.0.0"
build-path := "build"
build-file := build-path / "juicy-breakout-v" + version
main := "main.cpp"

run:
  just build
  ./{{build-file}}

build:
  mkdir -p {{build-path}}
  {{cxx}} {{flags}} {{build-file}} {{main}}

clean:
  rm build/** && rm build
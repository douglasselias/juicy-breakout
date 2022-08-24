#include "raylib.h"
#include <iostream>

int main(void)
{
  const int screenWidth = 1080;
  const int screenHeight = 720;

  InitWindow(screenWidth, screenHeight, "Juicy Breakout");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  MaximizeWindow();
  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Hello World!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
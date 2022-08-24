#include "raylib.h"
#include "raymath.h"

int main(void)
{
  const int screenWidth = 1080;
  const int screenHeight = 720;

  InitWindow(screenWidth, screenHeight, "Juicy Breakout");
  SetTargetFPS(60);

  Rectangle player = {};
  player.width = 200;
  player.height = 30;
  player.x = GetScreenWidth() / 2 - player.width / 2;
  player.y = GetScreenHeight() - player.height;

  while (!WindowShouldClose())
  {
    float mousePosition = GetMouseX() - player.width / 2;
    player.x = Clamp(mousePosition, 0, GetScreenWidth() - player.width);

    BeginDrawing();

    ClearBackground(BLACK);

    DrawRectangleRec(player, WHITE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
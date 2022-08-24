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

  int ballRadius = 10;
  int ballRadii = ballRadius / 2;
  Vector2 ballPosition = {};
  ballPosition.x = GetScreenWidth() / 2;
  ballPosition.y = GetScreenHeight() / 2;
  Vector2 ballSpeed = {};
  ballSpeed.x = 300;
  ballSpeed.y = 300;

  while (!WindowShouldClose())
  {
    float mousePositionWithPlayerOffset = GetMouseX() - player.width / 2;
    player.x = Clamp(mousePositionWithPlayerOffset, 0, GetScreenWidth() - player.width);

    if (ballPosition.x > GetScreenWidth() - ballRadii || ballPosition.x < 0 + ballRadii)
      ballSpeed.x *= -1;
    if (ballPosition.y > GetScreenHeight() - ballRadii || ballPosition.y < 0 + ballRadii)
      ballSpeed.y *= -1;

    if (CheckCollisionCircleRec(ballPosition, ballRadius, player))
    {
      ballSpeed.y *= -1;
    }

    ballPosition.x += ballSpeed.x * GetFrameTime();
    ballPosition.y += ballSpeed.y * GetFrameTime();

    BeginDrawing();

    ClearBackground(BLACK);

    DrawRectangleRec(player, WHITE);
    DrawCircleV(ballPosition, ballRadius, WHITE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
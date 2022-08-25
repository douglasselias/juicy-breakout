#include "raylib.h"
#include "raymath.h"

#include <vector>

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

  std::vector<Rectangle> bricks;
  int brickWidth = 100;
  int brickHeight = 30;
  int brickSpacing = 20;
  int columns = 7;
  int lines = 4;
  int topOffset = 50;
  int leftOffset = GetScreenWidth() / 2 - (columns * brickWidth + columns * brickSpacing) / 2;

  for (int i = 0; i < columns; i++)
    for (int j = 0; j < lines; j++)
    {
      Rectangle brick = {};
      brick.width = brickWidth;
      brick.height = brickHeight;
      brick.x = (i * brick.width + i * brickSpacing) + leftOffset;
      brick.y = (j * brick.height + j * brickSpacing) + topOffset;

      bricks.push_back(brick);
    }

  while (!WindowShouldClose())
  {
    float mousePositionWithPlayerOffset = GetMouseX() - player.width / 2;
    player.x = Clamp(mousePositionWithPlayerOffset, 0, GetScreenWidth() - player.width);

    if (ballPosition.x > GetScreenWidth() - ballRadii || ballPosition.x < 0 + ballRadii)
      ballSpeed.x *= -1;
    if (ballPosition.y > GetScreenHeight() - ballRadii || ballPosition.y < 0 + ballRadii)
      ballSpeed.y *= -1;

    if (CheckCollisionCircleRec(ballPosition, ballRadius, player))
      ballSpeed.y *= -1;

    ballPosition.x += ballSpeed.x * GetFrameTime();
    ballPosition.y += ballSpeed.y * GetFrameTime();

    for (auto &brick : bricks)
    {
      if (CheckCollisionCircleRec(ballPosition, ballRadius, brick))
      {
        float verticalHitboxWidth = brick.width / 3;
        float horizontalHitboxHeight = brick.height / 3;

        Rectangle bottomHitbox = {};
        bottomHitbox.width = brick.width - verticalHitboxWidth * 2;
        bottomHitbox.height = horizontalHitboxHeight;
        bottomHitbox.x = brick.x + verticalHitboxWidth;
        bottomHitbox.y = brick.y + brick.height - bottomHitbox.height;

        if (CheckCollisionCircleRec(ballPosition, ballRadius, bottomHitbox))
        {
          DrawText("Hit Bottom", 0, 0, 26, WHITE);
          ballSpeed.y *= -1;
          brick = {};
          break;
        }

        Rectangle topHitbox = {};
        topHitbox.width = brick.width - verticalHitboxWidth * 2;
        topHitbox.height = horizontalHitboxHeight;
        topHitbox.x = brick.x + verticalHitboxWidth;
        topHitbox.y = brick.y;

        if (CheckCollisionCircleRec(ballPosition, ballRadius, topHitbox))
        {
          DrawText("Hit Top", 0, 0, 26, WHITE);
          ballSpeed.y *= -1;
          brick = {};
          break;
        }

        Rectangle leftHitbox = {};
        leftHitbox.width = verticalHitboxWidth;
        leftHitbox.height = brick.height - horizontalHitboxHeight * 2;
        leftHitbox.x = brick.x;
        leftHitbox.y = brick.y + horizontalHitboxHeight;

        if (CheckCollisionCircleRec(ballPosition, ballRadius, leftHitbox))
        {
          DrawText("Hit Left", 0, 0, 26, WHITE);
          ballSpeed.x *= -1;
          brick = {};
          break;
        }

        Rectangle rightHitbox = {};
        rightHitbox.width = verticalHitboxWidth;
        rightHitbox.height = brick.height - horizontalHitboxHeight * 2;
        rightHitbox.x = brick.x + brick.width - rightHitbox.width;
        rightHitbox.y = brick.y + horizontalHitboxHeight;

        if (CheckCollisionCircleRec(ballPosition, ballRadius, rightHitbox))
        {
          DrawText("Hit Right", 0, 0, 26, WHITE);
          ballSpeed.x *= -1;
          brick = {};
        }
      }
    }

    BeginDrawing();

    ClearBackground(BLACK);

    DrawRectangleRec(player, WHITE);
    DrawCircleV(ballPosition, ballRadius, WHITE);

    for (auto &brick : bricks)
      DrawRectangleRec(brick, WHITE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
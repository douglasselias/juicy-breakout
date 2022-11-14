#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <entt/entt.hpp>

float easeElasticOut(float t, float b, float c, float d)
{
  if (t == 0.0f)
    return b;
  if ((t /= d) == 1.0f)
    return (b + c);

  float p = d * 0.3f;
  float a = c;
  float s = p / 4.0f;

  return (a * powf(2.0f, -10.0f * t) * sinf((t * d - s) * (2.0f * PI) / p) + c + b);
}

int main(void)
{
  int textFinalY = 10;
  int textInitialY = 0;
  enum FLAGS
  {
    COLOR = 2,
    TWEENING = 4,
    SQUASH_STRETCH = 8,
  };
  int currentFlag = COLOR;

  int framesCounter = 0;
  bool isTweeningAnimationFinished = false;

  const int screenWidth = 1080;
  const int screenHeight = 720;

  InitWindow(screenWidth, screenHeight, "Juicy Breakout");
  SetTargetFPS(60);

  int basePlayerWidth = 200;
  int basePlayerHeight = 30;
  Rectangle player = {
      .width = basePlayerWidth,
      .height = basePlayerHeight,
      .x = GetScreenWidth() / 2 - player.width / 2,
      .y = GetScreenHeight() - player.height,
  };
  int finalPlayerY = player.y;

  int baseBallRadius = 10;
  int ballRadius = baseBallRadius;
  int ballRadii = ballRadius / 2;
  Vector2 ballPosition = {
      .x = GetScreenWidth() / 2,
      .y = GetScreenHeight() / 2,
  };
  Vector2 ballSpeed = {
      .x = 300,
      .y = 300};
  bool startBallAnim = false;

  std::vector<Rectangle> bricksWithOriginalPosition;
  std::vector<Rectangle> bricks;
  int brickWidth = 100;
  int brickHeight = 30;
  int brickSpacing = 20;
  int columns = 7;
  int lines = 4;
  int topOffset = 50;
  int leftOffset = GetScreenWidth() / 2 - (columns * brickWidth + columns * brickSpacing) / 2;

  for (int i = 0; i < columns; i++)
  {
    for (int j = 0; j < lines; j++)
    {
      Rectangle brick = {
          .width = brickWidth,
          .height = brickHeight,
          .x = (i * brick.width + i * brickSpacing) + leftOffset,
          .y = (j * brick.height + j * brickSpacing) + topOffset,
      };
      bricks.push_back(brick);
      bricksWithOriginalPosition.push_back(brick);
    }
  }

  uint score = 0;
  uint lives = 3;

  while (!WindowShouldClose())
  {
    if (IsKeyPressed(KEY_N) && currentFlag < 16)
      currentFlag = (currentFlag << 1) + 1;
    if (IsKeyPressed(KEY_B) && currentFlag > 1)
      currentFlag >>= 1;

    if (currentFlag < 5)
      isTweeningAnimationFinished = false;

    float mousePositionWithPlayerOffset = GetMouseX() - player.width / 2;
    player.x = Clamp(mousePositionWithPlayerOffset, 0, GetScreenWidth() - player.width);

    if (ballPosition.x > GetScreenWidth() - ballRadii || ballPosition.x < 0 + ballRadii)
      ballSpeed.x *= -1;
    if (ballPosition.y > GetScreenHeight() - ballRadii || ballPosition.y < 0 + ballRadii)
    {
      ballSpeed.y *= -1;
      lives--;
    }

    if (CheckCollisionCircleRec(ballPosition, ballRadius, player))
      ballSpeed.y *= -1;

    ballPosition.x += ballSpeed.x * GetFrameTime();
    ballPosition.y += ballSpeed.y * GetFrameTime();

    for (auto &brick : bricks)
    {
      if (CheckCollisionCircleRec(ballPosition, ballRadius, brick))
      {
        startBallAnim = true;

        float verticalHitboxWidth = brick.width / 3;
        float horizontalHitboxHeight = brick.height / 3;

        Rectangle bottomHitbox = {
            .width = brick.width,
            .height = horizontalHitboxHeight,
            .x = brick.x,
            .y = brick.y + brick.height - bottomHitbox.height,
        };

        if (CheckCollisionCircleRec(ballPosition, ballRadius, bottomHitbox))
        {
          ballSpeed.y *= -1;
          brick = {};
          score += 100;
          break;
        }

        Rectangle topHitbox = {
            .width = brick.width,
            .height = horizontalHitboxHeight,
            .x = brick.x,
            .y = brick.y,
        };

        if (CheckCollisionCircleRec(ballPosition, ballRadius, topHitbox))
        {
          ballSpeed.y *= -1;
          brick = {};
          score += 100;
          break;
        }

        Rectangle leftHitbox = {
            .width = verticalHitboxWidth,
            .height = brick.height - horizontalHitboxHeight * 2,
            .x = brick.x,
            .y = brick.y + horizontalHitboxHeight,
        };

        if (CheckCollisionCircleRec(ballPosition, ballRadius, leftHitbox))
        {
          ballSpeed.x *= -1;
          brick = {};
          score += 100;
          break;
        }

        Rectangle rightHitbox = {
            .width = verticalHitboxWidth,
            .height = brick.height - horizontalHitboxHeight * 2,
            .x = brick.x + brick.width - rightHitbox.width,
            .y = brick.y + horizontalHitboxHeight,
        };

        if (CheckCollisionCircleRec(ballPosition, ballRadius, rightHitbox))
        {
          ballSpeed.x *= -1;
          brick = {};
          score += 100;
          break;
        }
      }
    }

    if (startBallAnim && currentFlag & SQUASH_STRETCH)
    {
      framesCounter++;

      if (framesCounter < 60 / 2)
        ballRadius = easeElasticOut((float)framesCounter, 0, 20, 30);
      else
        ballRadius = easeElasticOut((float)framesCounter, 0, 10, 30);

      if (framesCounter >= 60)
      {
        framesCounter = 0;
        startBallAnim = false;
        ballRadius = baseBallRadius;
      }
    }

    if (!isTweeningAnimationFinished && currentFlag & TWEENING)
    {
      framesCounter++;

      player.y = easeElasticOut((float)framesCounter, 0, finalPlayerY, 120);

      for (int i = 0; i < bricks.size(); i++)
        bricks[i].y = easeElasticOut((float)framesCounter, 0, bricksWithOriginalPosition[i].y, 120);

      textInitialY = easeElasticOut((float)framesCounter, 0, textFinalY, 120);

      if (framesCounter >= 120)
      {
        framesCounter = 0;
        isTweeningAnimationFinished = true;
      }
    }

    Vector2 mouseDelta = GetMouseDelta();
    float absMouseDeltaX = abs(mouseDelta.x);

    if (absMouseDeltaX > 50)
    {
      player.width = basePlayerWidth + 20;
      player.height = basePlayerHeight - 15;
      player.y = finalPlayerY + 15 / 2;
    }
    else
    {
      player.width = basePlayerWidth;
      player.height = basePlayerHeight;
      player.y = finalPlayerY;
    }

    BeginDrawing();

    ClearBackground(BLACK);

    DrawRectangleRec(player, currentFlag & COLOR ? DARKBLUE : WHITE);
    DrawCircleV(ballPosition, ballRadius, currentFlag & COLOR ? MAROON : WHITE);

    for (auto &brick : bricks)
      DrawRectangleRec(brick, currentFlag & COLOR ? GOLD : WHITE);

    DrawText(TextFormat("Score %d", score), 20, textInitialY, 26, currentFlag & COLOR ? SKYBLUE : WHITE);
    DrawText(TextFormat("Lives %d", lives), GetScreenWidth() - 100, textInitialY, 26, currentFlag & COLOR ? SKYBLUE : WHITE);

    DrawText(TextFormat("Flags %d", currentFlag), GetScreenWidth() / 2, textInitialY, 26, currentFlag & COLOR ? SKYBLUE : WHITE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
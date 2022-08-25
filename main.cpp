#include "raylib.h"
#include "raymath.h"

#include <vector>

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
  };
  int currentFlag = 1;

  int framesCounter = 0;
  bool isTweeningAnimationFinished = false;

  const int screenWidth = 1080;
  const int screenHeight = 720;

  InitWindow(screenWidth, screenHeight, "Juicy Breakout");
  SetTargetFPS(60);

  Rectangle player = {};
  player.width = 200;
  player.height = 30;
  player.x = GetScreenWidth() / 2 - player.width / 2;
  player.y = GetScreenHeight() - player.height;
  int finalPlayerY = player.y;

  int ballRadius = 10;
  int ballRadii = ballRadius / 2;
  Vector2 ballPosition = {};
  ballPosition.x = GetScreenWidth() / 2;
  ballPosition.y = GetScreenHeight() / 2;
  Vector2 ballSpeed = {};
  ballSpeed.x = 300;
  ballSpeed.y = 300;

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
    for (int j = 0; j < lines; j++)
    {
      Rectangle brick = {};
      brick.width = brickWidth;
      brick.height = brickHeight;
      brick.x = (i * brick.width + i * brickSpacing) + leftOffset;
      brick.y = (j * brick.height + j * brickSpacing) + topOffset;

      bricks.push_back(brick);
      bricksWithOriginalPosition.push_back(brick);
    }

  int score = 0;
  int lives = 3;

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
        float verticalHitboxWidth = brick.width / 3;
        float horizontalHitboxHeight = brick.height / 3;

        Rectangle bottomHitbox = {};
        bottomHitbox.width = brick.width;
        bottomHitbox.height = horizontalHitboxHeight;
        bottomHitbox.x = brick.x;
        bottomHitbox.y = brick.y + brick.height - bottomHitbox.height;

        if (CheckCollisionCircleRec(ballPosition, ballRadius, bottomHitbox))
        {
          ballSpeed.y *= -1;
          brick = {};
          score += 100;
          break;
        }

        Rectangle topHitbox = {};
        topHitbox.width = brick.width;
        topHitbox.height = horizontalHitboxHeight;
        topHitbox.x = brick.x;
        topHitbox.y = brick.y;

        if (CheckCollisionCircleRec(ballPosition, ballRadius, topHitbox))
        {
          ballSpeed.y *= -1;
          brick = {};
          score += 100;
          break;
        }

        Rectangle leftHitbox = {};
        leftHitbox.width = verticalHitboxWidth;
        leftHitbox.height = brick.height - horizontalHitboxHeight * 2;
        leftHitbox.x = brick.x;
        leftHitbox.y = brick.y + horizontalHitboxHeight;

        if (CheckCollisionCircleRec(ballPosition, ballRadius, leftHitbox))
        {
          ballSpeed.x *= -1;
          brick = {};
          score += 100;
          break;
        }

        Rectangle rightHitbox = {};
        rightHitbox.width = verticalHitboxWidth;
        rightHitbox.height = brick.height - horizontalHitboxHeight * 2;
        rightHitbox.x = brick.x + brick.width - rightHitbox.width;
        rightHitbox.y = brick.y + horizontalHitboxHeight;

        if (CheckCollisionCircleRec(ballPosition, ballRadius, rightHitbox))
        {
          ballSpeed.x *= -1;
          brick = {};
          score += 100;
          break;
        }
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
        isTweeningAnimationFinished = 1;
      }
    }

    BeginDrawing();

    ClearBackground(BLACK);

    DrawRectangleRec(player,  currentFlag & COLOR ? DARKBLUE : WHITE);
    DrawCircleV(ballPosition, ballRadius,  currentFlag & COLOR ? MAROON : WHITE);

    for (auto &brick : bricks)
      DrawRectangleRec(brick, currentFlag & COLOR ? GOLD : WHITE);

    DrawText(TextFormat("Score %d", score), 20, textInitialY, 26,  currentFlag & COLOR ? SKYBLUE : WHITE);
    DrawText(TextFormat("Lives %d", lives), GetScreenWidth() - 100, textInitialY, 26, currentFlag & COLOR ? SKYBLUE : WHITE);

    DrawText(TextFormat("Flags %d", currentFlag), GetScreenWidth() / 2, textInitialY, 26, currentFlag & COLOR ? SKYBLUE : WHITE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
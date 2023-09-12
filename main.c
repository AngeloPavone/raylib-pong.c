#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct Ball {
  float x, y;
  float radius;
  float speedX, speedY;
  Vector2 position;
};

struct Paddle {
  float y, x;
  float speed;
  float height, width;
  float position;
  Rectangle rec;
};

int draw_left_paddle(size_t centerX, size_t centerY, float paddle_height, float paddle_width, float paddleY) {
  if (centerX > 0 && centerY > 0 && paddle_height > 0 && paddle_width > 0) {
    DrawRectangle(GetScreenWidth() - (GetScreenWidth() - paddle_width), paddleY - paddle_height / 2, paddle_width, paddle_height, WHITE);
    return 0;
  } else {
    perror("\033[31m ERROR: Value of centerX, centerY, paddle_height or "
           "paddle_width "
           "was less than 0; value should be greater than 0");
    exit(1);
  }
}

int draw_right_paddle(size_t centerX, size_t centerY, float paddle_height, float paddle_width, float paddleY) {
  if (centerX > 0 && centerY > 0 && paddle_height > 0 && paddle_width > 0) { DrawRectangle(GetScreenWidth() - (paddle_width + 10), paddleY - paddle_height / 2, paddle_width, paddle_height, WHITE);
    return 0;
  } else {
    perror("\033[31m ERROR: Value of centerX, centerY, paddle_height or "
           "paddle_width "
           "was less than 0; value should be greater than 0");
    exit(1);
  }
}

int main(void) {
  InitWindow(800, 800, "Pong");
  SetTargetFPS(60);

  float delta = 100.0f;

  struct Paddle leftPaddle;
  leftPaddle.x = 20;
  leftPaddle.y = GetScreenHeight() / 2.0f;
  leftPaddle.speed = 3.0f;
  leftPaddle.height = 100;
  leftPaddle.width = 10;

  struct Paddle rightPaddle;
  rightPaddle.x = GetScreenWidth() - 20;
  rightPaddle.y = GetScreenHeight() / 2.0f;
  rightPaddle.speed = 3.0f;
  rightPaddle.height = 100;
  rightPaddle.width = 10;

  struct Ball ball;
  ball.radius = 5.0f;
  ball.x = GetScreenWidth() / 2.0f;
  ball.y = GetScreenHeight() / 2.0f;

  if (IsWindowReady()) {
    while (!WindowShouldClose()) {
      size_t centerX = GetScreenWidth() / 2;
      size_t centerY = GetScreenHeight() / 2;
      const char* winner_text = NULL;
      rightPaddle.rec = (Rectangle){rightPaddle.x - rightPaddle.width / 2, rightPaddle.y - rightPaddle.height / 2, rightPaddle.width, rightPaddle.height};
      leftPaddle.rec = (Rectangle){leftPaddle.x - leftPaddle.width / 2, leftPaddle.y - leftPaddle.height / 2, leftPaddle.width, leftPaddle.height};
      ball.position = (Vector2){ball.x, ball.y};


      if (IsKeyDown(KEY_W)) {
        leftPaddle.y -= (leftPaddle.speed * GetFrameTime() * delta);
      }

      if (IsKeyDown(KEY_S)) {
        leftPaddle.y += (leftPaddle.speed * GetFrameTime() * delta);
      }

      if (IsKeyDown(KEY_UP)) {
        rightPaddle.y -= (rightPaddle.speed * GetFrameTime() * delta);
      }

      if (IsKeyDown(KEY_DOWN)) {
        rightPaddle.y += (rightPaddle.speed * GetFrameTime() * delta);
      }

      draw_left_paddle(centerX, centerY, leftPaddle.height, leftPaddle.width, leftPaddle.y);
      draw_right_paddle(centerX, centerY, rightPaddle.height, rightPaddle.width, rightPaddle.y);

      ball.x += (ball.speedX * GetFrameTime() * delta);
      ball.y += (ball.speedY * GetFrameTime() * delta);

      if (ball.y < 0) {
        ball.y = 0;
        ball.speedY *= -1;
      }
      if (ball.y > GetScreenHeight()) {
        ball.y = GetScreenHeight();
        ball.speedY *= -1;
      }

      if (leftPaddle.y < 0 + leftPaddle.height / 2) {
        leftPaddle.y = 0 + leftPaddle.height / 2;
      }
      if (leftPaddle.y > GetScreenHeight() - leftPaddle.height / 2) {
        leftPaddle.y = GetScreenHeight() - leftPaddle.height / 2;
      }

      if (rightPaddle.y > GetScreenHeight() - rightPaddle.height / 2) {
        rightPaddle.y = GetScreenHeight() - rightPaddle.height / 2;
      }
      if (rightPaddle.y < 0 + rightPaddle.height / 2) {
        rightPaddle.y = 0 + rightPaddle.height / 2;
      }

      if (CheckCollisionCircleRec(ball.position, ball.radius, leftPaddle.rec)) {
        if (ball.speedX < 0) {
          ball.speedX *= -1.1f;
          ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
        }
      }
      if (CheckCollisionCircleRec(ball.position, ball.radius, rightPaddle.rec)) {
        if (ball.speedX > 0) {
          ball.speedX *= -1.1f;
          ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
        }
      }

      DrawCircle(ball.x, ball.y, ball.radius, WHITE);

      if (ball.x > GetScreenWidth()) {
        winner_text = "Left Player Wins!";
      }
      if (ball.x < 0) {
        winner_text = "Right Player Wins!";
      }

      if ((winner_text && IsKeyPressed(KEY_SPACE)) || IsKeyPressed(KEY_SPACE)) {
        ball.x = centerX;
        ball.y = centerY;
        ball.speedX = 0;
        ball.speedY = 0;
        leftPaddle.x = 20;
        leftPaddle.y = GetScreenHeight() / 2.0f;
        rightPaddle.x = GetScreenWidth() - 20;
        rightPaddle.y = GetScreenHeight() / 2.0f;
        srand(time(NULL));
        size_t x = rand() % 2;
        if (x > 0) {
          ball.speedX = 2.0f * 1;
        } else {
          ball.speedX = 2.0f * -1;
        }
        ball.speedY = 0.0f;
        winner_text = NULL;
      }

      BeginDrawing();
      ClearBackground(BLACK);

      size_t text_width = MeasureText(winner_text, 60);
      if (winner_text != NULL) {
        DrawText(winner_text, GetScreenWidth() / 2 - text_width / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
      }

      DrawFPS(10, 10);
      EndDrawing();
    }
  }
  CloseWindow();
  return 0;
}

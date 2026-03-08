#include <raylib.h>

const int FPS = 60;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const char *TITLE = "raylib - hungrycircle";

typedef struct Actor {
  Vector2 position;
  Vector2 heading;
  float size;
  Color color;
  float speed;
} Actor;

void draw_actor(Actor *actor, float deltaTime);

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  SetTargetFPS(FPS);

  Actor actor = {
      .position = {SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0},
      .heading = {1, 0},
      .size = 30.0f,
      .color = RED,
      .speed = 200.0f,
  };

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();
    BeginDrawing();
    ClearBackground(WHITE);
    DrawFPS(10, 10);
    draw_actor(&actor, deltaTime);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

void draw_actor(Actor *actor, float deltaTime) {
  Vector2 newPosition = actor->position;

  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
    newPosition.y -= actor->speed * deltaTime;
  }
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
    newPosition.x -= actor->speed * deltaTime;
  }
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
    newPosition.y += actor->speed * deltaTime;
  }
  if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
    newPosition.x += actor->speed * deltaTime;
  }

  actor->position = newPosition;

  DrawCircle(actor->position.x, actor->position.y, actor->size, actor->color);
}

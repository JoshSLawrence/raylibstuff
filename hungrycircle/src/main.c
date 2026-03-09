#include <raylib.h>
#include <raymath.h>

const int FPS = 60;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const char *TITLE = "raylib - hungrycircle";
const float MOVEMENT_SPEED = 400.0f;
const int MAX_FOOD = 10;
const Vector2 STARTING_POSITION = {.x = 0, .y = 0};

typedef struct Actor {
  Vector2 position;
  Vector2 heading;
  float size;
  Color color;
  float speed;
} Actor;

typedef struct Food {
  Vector2 position;
  Color color;
  float size;
  bool active;
} Food;

void draw_actor(Actor *actor, float deltaTime);
Vector2 spawn_food(Food *food, Actor *actor);
void draw_food(Food food[]);
void check_collision(Actor *actor, Food food[]);

int main() {
  int score = 0;

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  SetTargetFPS(FPS);

  Actor actor = {
      .position = {SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0},
      .heading = {1, 0},
      .size = 30.0f,
      .color = RED,
      .speed = MOVEMENT_SPEED,
  };

  Food food[MAX_FOOD];
  for (int i = 0; i < MAX_FOOD; i++) {
    food[i].color = GREEN;
    food[i].size = 15.0f;
    food[i].position = spawn_food(&food[i], &actor);
    food[i].active = false;
  }

  for (int i = 0; i < MAX_FOOD; i++) {
    TraceLog(
        LOG_INFO, "Food %d position x: %f y: %f", i + 1, food[i].position.x,
        food[i].position.y
    );
  }

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();

    for (int i = 0; i < MAX_FOOD; i++) {
      if (CheckCollisionCircles(
              actor.position, actor.size, food[i].position, food[i].size
          )) {
        food[i].position = spawn_food(&food[i], &actor);
        score += 100;
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);
    const char *scoreText = TextFormat("score: %d", score);
    int textWidth = MeasureText(scoreText, 20);
    draw_food(food);
    draw_actor(&actor, deltaTime);
    DrawFPS(10, 10);
    DrawText(
        TextFormat("Score: %d", score), GetScreenWidth() - textWidth - 10,
        GetScreenHeight() - 30, 20, YELLOW
    );
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

  if (newPosition.x < -actor->size) {
    newPosition.x = GetScreenWidth() + actor->size;
  }
  if (newPosition.y < -actor->size) {
    newPosition.y = GetScreenHeight() + actor->size;
  }
  if (newPosition.x > GetScreenWidth() + actor->size) {
    newPosition.x = -actor->size;
  }
  if (newPosition.y > GetScreenHeight() + actor->size) {
    newPosition.y = -actor->size;
  }

  actor->position = newPosition;

  DrawCircle(actor->position.x, actor->position.y, actor->size, actor->color);
}

Vector2 spawn_food(Food *food, Actor *actor) {
  const float minDistance = food->size + actor->size;

  while (true) {
    Vector2 randomPosition = {
        .x = GetRandomValue(0, GetScreenWidth()),
        .y = GetRandomValue(0, GetScreenHeight()),
    };

    // Check distance from actor
    if (Vector2Distance(randomPosition, actor->position) < minDistance) {
      continue; // Too close to actor, try again
    }

    return randomPosition;
  }
}

void draw_food(Food food[]) {
  for (int i = 0; i < MAX_FOOD; i++) {
    DrawCircleV(food[i].position, food[i].size, food[i].color);
  }
}

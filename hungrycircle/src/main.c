#include <raylib.h>
#include <raymath.h>

const int FPS = 60;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const char *TITLE = "raylib - hungrycircle";
const float MOVEMENT_SPEED = 450.0f;
const int MAX_FOOD = 5;
const Vector2 STARTING_POSITION = {.x = 0, .y = 0};
const int MAX_SCORE = 10000;
const int POINTS_PER_CIRCLE = 100;

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
void draw_victory_screen();
void draw_score(int score);

int main() {
  bool gameover = false;
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
    food[i].color = BLUE;
    food[i].size = 15.0f;
    food[i].position = spawn_food(&food[i], &actor);
    food[i].active = false;
  }

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();

    for (int i = 0; i < MAX_FOOD; i++) {
      if (gameover)
        break;

      if (CheckCollisionCircles(
              actor.position, actor.size, food[i].position, food[i].size
          )) {
        food[i].position = spawn_food(&food[i], &actor);
        score += POINTS_PER_CIRCLE;
        TraceLog(LOG_INFO, "Score +100");
        actor.size *= 1.05;
        TraceLog(LOG_INFO, TextFormat("Actor Size: %f", actor.size));
      }

      if (score >= MAX_SCORE)
        gameover = true;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    if (gameover) {
      draw_victory_screen();
    } else {
      draw_food(food);
      draw_actor(&actor, deltaTime);
    }
    DrawFPS(10, 10);
    draw_score(score);
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
  const int maxAttempts = 1000;

  for (int i = 0; i < maxAttempts; i++) {
    Vector2 randomPosition = {
        .x = GetRandomValue(0, GetScreenWidth()),
        .y = GetRandomValue(0, GetScreenHeight()),
    };

    // Check distance from actor
    if (Vector2Distance(randomPosition, actor->position) < minDistance) {
      continue; // Too close to actor, try again
    }

    TraceLog(
        LOG_INFO, "Spawned Food in %d attempts - position x: %f y: %f", i + 1,
        randomPosition.x, randomPosition.y
    );
    return randomPosition;
  }
  TraceLog(
      LOG_INFO, "[FALLBACK] Spawned Food - position x: %f y: %f", 0.0, 0.0
  );
  return (Vector2){.x = 0.0, .y = 0.0};
}

void draw_food(Food food[]) {
  for (int i = 0; i < MAX_FOOD; i++) {
    DrawCircleV(food[i].position, food[i].size, food[i].color);
  }
}

void draw_victory_screen() {
  int fontSize = 50;
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RED);
  const char *victoryText = "VICTORY";
  int textWidth = MeasureText(victoryText, fontSize);
  DrawText(
      victoryText, (GetScreenWidth() / 2) - (textWidth / 2),
      (GetScreenHeight() / 2) - (fontSize / 2), fontSize, GREEN
  );
}

void draw_score(int score) {
  const char *scoreText = TextFormat("score: %d", score);
  int textWidth = MeasureText(scoreText, 20);
  DrawText(
      TextFormat("Score: %d", score), GetScreenWidth() - textWidth - 10,
      GetScreenHeight() - 30, 20, YELLOW
  );
}

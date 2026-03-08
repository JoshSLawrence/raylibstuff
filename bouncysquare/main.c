#include <math.h>
#include <raylib.h>

const int FPS = 60;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const char *WINDOW_TITLE = "raylib lab - bouncing square";
const float SPEED = 500.0f;
const int SIZE = 80;
const int STARTING_POSITION[] = {0, SCREEN_HEIGHT / 2 - SIZE / 2};

typedef struct Square {
  Rectangle rectangle;
  Vector2 heading;
  float speed;
  bool selected;
  Vector2 lastMousePos;
  int sampleCount;
} Square;

void move_square(Square *square, float deltaTime);
bool is_mouse_on_square(Square *square);

int main(void) {

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(FPS);

  Square square = {
      .rectangle =
          {
              .x = STARTING_POSITION[0],
              .y = STARTING_POSITION[1],
              .width = SIZE,
              .height = SIZE,
          },
      .heading =
          {
              .x = 1.0f,
              .y = 0.0f,
          },
      .speed = SPEED,
      .selected = false,
      .lastMousePos = {0, 0},
      .sampleCount = 0,
  };

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();

    if (square.selected && IsMouseButtonDown(MOUSE_LEFT_BUTTON) ||
        is_mouse_on_square(&square)) {
      square.selected = true;
    } else {
      square.selected = false;
    }

    move_square(&square, deltaTime);
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangleRec(square.rectangle, RED);
    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

bool is_mouse_on_square(Square *square) {
  if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    return false;
  }

  Vector2 mousePosition = GetMousePosition();

  if (mousePosition.x >= square->rectangle.x &&
      mousePosition.x <= square->rectangle.x + square->rectangle.width &&
      mousePosition.y >= square->rectangle.y &&
      mousePosition.y <= square->rectangle.y + square->rectangle.height) {
    TraceLog(LOG_INFO, "Mouse button is on the square!");
    return true;
  }

  return false;
}

void move_square(Square *square, float deltaTime) {
  Vector2 originalPosition = {
      .x = square->rectangle.x,
      .y = square->rectangle.y,
  };
  Vector2 newPosition = originalPosition;

  if (square->selected) {
    Vector2 mousePosition = GetMousePosition();

    newPosition.x = mousePosition.x - square->rectangle.width / 2;
    newPosition.y = mousePosition.y - square->rectangle.height / 2;

    // Calculate velocity from mouse movement
    if (square->sampleCount > 0) {
      float dx = mousePosition.x - square->lastMousePos.x;
      float dy = mousePosition.y - square->lastMousePos.y;

      // Smooth the heading by averaging with previous direction
      const float smoothing = 0.3f;
      square->heading.x =
          square->heading.x * (1.0f - smoothing) + dx * smoothing;
      square->heading.y =
          square->heading.y * (1.0f - smoothing) + dy * smoothing;

      // Normalize the heading vector
      float magnitude = sqrtf(
          square->heading.x * square->heading.x +
          square->heading.y * square->heading.y
      );
      if (magnitude > 0.01f) {
        square->heading.x /= magnitude;
        square->heading.y /= magnitude;
      }
    }

    square->lastMousePos = mousePosition;
    square->sampleCount++;
  }

  // Boundary checking with heading reversal
  if (newPosition.x > GetScreenWidth() - square->rectangle.width) {
    newPosition.x = GetScreenWidth() - square->rectangle.width;
    square->heading.x = -fabsf(square->heading.x);
  }

  if (newPosition.x < 0) {
    newPosition.x = 0;
    square->heading.x = fabsf(square->heading.x);
  }

  if (newPosition.y > GetScreenHeight() - square->rectangle.height) {
    newPosition.y = GetScreenHeight() - square->rectangle.height;
    square->heading.y = -fabsf(square->heading.y);
  }

  if (newPosition.y < 0) {
    newPosition.y = 0;
    square->heading.y = fabsf(square->heading.y);
  }

  if (square->selected) {
    square->rectangle.x = newPosition.x;
    square->rectangle.y = newPosition.y;
    return;
  }

  // Move based on normalized heading vector
  newPosition.x += square->heading.x * square->speed * deltaTime;
  newPosition.y += square->heading.y * square->speed * deltaTime;

  square->rectangle.x = newPosition.x;
  square->rectangle.y = newPosition.y;
}

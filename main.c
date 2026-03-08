#include <math.h>
#include <raylib.h>

const int FPS = 60;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const char *WINDOW_TITLE = "raylib lab - bouncing square";
const float SPEED = 500.0f;
const int SIZE = 80;
const int STARTING_POSITION[] = {0, SCREEN_HEIGHT / 2 - SIZE / 2};

typedef enum {
  NORTH,
  SOUTH,
  EAST,
  WEST,
  NONE,
} Direction;

typedef struct Heading {
  Direction vertical;
  Direction horizontal;
} Heading;

typedef struct Square {
  Rectangle rectangle;
  Heading heading;
  float speed;
  bool selected;
  int rise;
  int run;
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
              .vertical = NONE,
              .horizontal = EAST,
          },
      .speed = SPEED,
      .selected = false,
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

    int rise = newPosition.y - originalPosition.y;
    int run = newPosition.x - originalPosition.x;

    if (rise > 0 && run == 0) {
      square->heading.vertical = NORTH;
      square->heading.horizontal = NONE;
    } else if (rise < 0 && run == 0) {
      square->heading.vertical = SOUTH;
      square->heading.horizontal = NONE;
    } else if (rise == 0 && run > 0) {
      square->heading.vertical = NONE;
      square->heading.horizontal = EAST;
    } else if (rise == 0 && run < 0) {
      square->heading.vertical = NONE;
      square->heading.horizontal = WEST;
    } else if (rise > 0 && run > 0) {
      square->heading.vertical = NORTH;
      square->heading.horizontal = EAST;
    } else if (rise > 0 && run < 0) {
      square->heading.vertical = NORTH;
      square->heading.horizontal = WEST;
    } else if (rise < 0 && run > 0) {
      square->heading.vertical = SOUTH;
      square->heading.horizontal = EAST;
    } else if (rise < 0 && run < 0) {
      square->heading.vertical = SOUTH;
      square->heading.horizontal = WEST;
    }
  }

  if (newPosition.x > GetScreenWidth() - square->rectangle.width) {
    newPosition.x = GetScreenWidth() - square->rectangle.width;
    square->heading.horizontal = WEST;
  }

  if (newPosition.x < 0) {
    newPosition.x = 0;
    square->heading.horizontal = EAST;
  }

  if (newPosition.y > GetScreenHeight() - square->rectangle.width) {
    newPosition.y = GetScreenHeight() - square->rectangle.height;
    square->heading.vertical = SOUTH;
  }

  if (newPosition.y < 0) {
    newPosition.y = 0;
    square->heading.vertical = NORTH;
  }

  if (square->selected) {
    square->rectangle.x = newPosition.x;
    square->rectangle.y = newPosition.y;
    return;
  }

  switch (square->heading.vertical) {
  case NORTH:
    newPosition.y += square->speed * deltaTime;
    break;
  case SOUTH:
    newPosition.y -= square->speed * deltaTime;
    break;
  default:
    break;
  }

  switch (square->heading.horizontal) {
  case EAST:
    newPosition.x += square->speed * deltaTime;
    break;
  case WEST:
    newPosition.x -= square->speed * deltaTime;
    break;
  default:
    break;
  }

  square->rectangle.x = newPosition.x;
  square->rectangle.y = newPosition.y;
}

#include <stdlib.h>

#include "ssd1306.h"
#include "snake.h"

#define SNAKE_LENGTH 10

Entity* snake_init(uint8_t* size) {
  static Entity pattern[SNAKE_LENGTH];

  uint8_t startX = 110;
  uint8_t startY = 20;

  for (uint8_t i = 0; i < SNAKE_LENGTH; i++) {
    pattern[i].position.x = startX + i;
    pattern[i].position.y = startY;

    // Clockwise movement
    pattern[i].velocity.x = 1;
    pattern[i].velocity.y = 1;
  }

  *size = SNAKE_LENGTH;

  return pattern;
}

void snake_animation_update(Entity* pattern, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    Entity e = pattern[i];

    uint8_t nextX = e.position.x + e.velocity.x;
    if (ssd1306_inBounds(nextX, e.position.y)) {
      e.position.x = nextX;
      pattern[i] = e;
      continue;
    }

    uint8_t nextY = e.position.y + e.velocity.y;
    if (ssd1306_inBounds(e.position.x, nextY)) {
      e.position.y = nextY;
      pattern[i] = e;
      continue;
    }

    // Invert directions as we could not move in either one
    e.velocity.x *= -1;
    e.velocity.y *= -1;

    pattern[i] = e;
  }
}

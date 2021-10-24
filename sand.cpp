#include <stdlib.h>
#include <math.h>
#include "ssd1306.h"
#include "uart.h"
#include "sand.h"

#define NUMBER_OF_PARTICLES 32
#define GRAVITY 10.f
#define TILT_AMP 4.f

Entity* sand_init(uint8_t* size) {
  static Entity pattern[NUMBER_OF_PARTICLES];

  for (uint16_t i = 0; i < NUMBER_OF_PARTICLES; i++) {
    pattern[i].position.x = i;
    pattern[i].position.y = 31;
  }

  *size = NUMBER_OF_PARTICLES;

  return pattern;
}

float clamp_f(float val, float min, float max) {
  if (val < min) return min;
  if (val > max) return max;
  return val;
}

bool is_empty(uint8_t x, uint8_t y, Entity* entities, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    Entity* e = &entities[i];

    if (e->position.x == x && e->position.y == y) {
      return false;
    }
  }

  return true;
}

void sand_update(Vec2_f* acceleration, float dt, Entity* entities, uint8_t size) {
  // Amplify x/y so that the acceleration in is mapped
  // from [0...1] to [0...TILT_AMP] so one does not have to
  // tilt too much before it affects the entities
  float ax = acceleration->x * TILT_AMP;
  float ay = acceleration->y * TILT_AMP;

  for (uint8_t i = 0; i < size; i++) {
    Entity* e = &entities[i];

    float minGravity = GRAVITY * -1.f;
    e->velocity.x = clamp_f(e->velocity.x + (ax * GRAVITY * dt), minGravity, GRAVITY);
    e->velocity.y = clamp_f(e->velocity.y + (ay * GRAVITY * dt), minGravity, GRAVITY);

    uint8_t dx = round(e->position.x + e->velocity.x);
    uint8_t dy = round(e->position.y + e->velocity.y);

    // Check if we can move where we want
    if (ssd1306_inBounds(dx, dy) && is_empty(dx, dy, entities, size)) {
      e->position.x = dx;
      e->position.y = dy;

      continue;
    }

    // Couldn't move where we wanted, half speed due to "impact"
    e->velocity.x /= 2.f;
    e->velocity.y /= 2.f;
  }
}

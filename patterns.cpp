#include <stdlib.h>
#include "ssd1306.h"
#include "patterns.h"

Entity* patterns_corners(uint8_t* size) {
  static Entity pattern[4];

  pattern[0].position.x = 0;
  pattern[0].position.y = 0;

  pattern[1].position.x = 127;
  pattern[1].position.y = 0;

  pattern[2].position.x = 0;
  pattern[2].position.y = 31;

  pattern[3].position.x = 127;
  pattern[3].position.y = 31;

  *size = 4;

  return pattern;
}

Entity* patterns_same_row(uint8_t* size) {
  static Entity pattern[12];

  pattern[0].position.x = 0;
  pattern[0].position.y = 0;

  pattern[1].position.x = 0;
  pattern[1].position.y = 2;

  pattern[2].position.x = 0;
  pattern[2].position.y = 4;

  pattern[3].position.x = 0;
  pattern[3].position.y = 6;

  pattern[4].position.x = 0;
  pattern[4].position.y = 8;

  pattern[5].position.x = 0;
  pattern[5].position.y = 10;

  pattern[6].position.x = 127;
  pattern[6].position.y = 21;

  pattern[7].position.x = 127;
  pattern[7].position.y = 23;

  pattern[8].position.x = 127;
  pattern[8].position.y = 25;

  pattern[9].position.x = 127;
  pattern[9].position.y = 27;

  pattern[10].position.x = 127;
  pattern[10].position.y = 29;

  pattern[11].position.x = 127;
  pattern[11].position.y = 31;

  *size = 12;

  return pattern;
}

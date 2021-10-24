#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include "vector.h"

typedef struct {
  Vec2_uint8 position;
  Vec2_f velocity; // TODO change to Vec2_uint8
} Entity;

#endif

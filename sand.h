#ifndef SAND_H
#define SAND_H

#include "stdint.h"
#include "vector.h"
#include "entity.h"

Entity* sand_init(uint8_t* size);
void sand_update(Vec2_f* acceleration, float dt, Entity* entities, uint8_t size);

#endif

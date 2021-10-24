#ifndef SNAKE_H
#define SNAKE_H

#include "stdint.h"
#include "entity.h"

Entity* snake_init(uint8_t* size);
void snake_animation_update(Entity* pattern, uint8_t size);

#endif

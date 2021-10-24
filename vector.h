#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

typedef struct {
  uint8_t x;
  uint8_t y;
} Vec2_uint8;

typedef struct {
  float x;
  float y;
} Vec2_f;

typedef struct {
  float x;
  float y;
  float z;
} Vec3_f;

#endif

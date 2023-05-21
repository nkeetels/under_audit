#ifndef __FIXEDMATH_H
#define __FIXEDMATH_H

#include <stdint.h>
#include "memory.h"

extern int16_t IWRAM_CODE _div(int a, int b);
extern int16_t IWRAM_CODE _atan2(int a, int b);
extern int16_t IWRAM_CODE _sqrt(int a);

#define SINLUT_SIZE 255
extern int16_t SinLUT[];
extern const unsigned short lut_spherical[256];

#define sin(x) SinLUT[x & SINLUT_SIZE]
#define cos(x) SinLUT[(64 + (x)) & SINLUT_SIZE]
#define spherical(x) lut_spherical[x & 255]
#define lerp(a, b, t) ((a * (255 - t) + b * t) >> 8)
#define abs(a) ((a) < (0) ? (-a) : (a))
#define max(a, b) a - ((a-b) & (a-b)>>31)
#define min(a, b) ((((a)-(b)) & 0x80000000) >> 31) ? (a) : (b)
#define clamp(x, a, b) (max(a, min(x, b)))
#define mod(a, b) (a) >= (b) ? (a) % (b) : (a)
#define sign(x) (((x) > 0) - ((x) < 0))
#define swap(x, y) x^=y; y^=x; x^=y
#define pow(x) (x) * (x)
#define div(a, b) _div(a, b)
#define atan2(a, b) _atan2(a, b)
#define sqrt(a) _sqrt(a)
#define prevent_zero(x) (((x) == 0) ? (++x) : (x))
#define unpack_color(b0, b1, color) (b0) = ((color) & 0xFF); (b1) = (((color) >> 8) & 0xFF)


#define screenoffset(x, y) ((y << 8) + (y << 6) + x)
#define mul120(x) ((x << 7) + (x << 4))

typedef struct vector2_t {
  int x, y;
} vector2_t;

typedef struct vector3_t {
  int x, y, z;
} vector3_t;

typedef struct plane_t {
  vector3_t normal;
  int d;
} plane_t;

typedef int matrix3x3_t[3][3];

unsigned rand();
void rotate(matrix3x3_t m, int x, int y, int z);
void normal(vector3_t *out, const vector3_t *a, const vector3_t *b, const vector3_t *c);
int square_magnitude(const vector3_t *v);
int magnitude(const vector3_t *v);


#endif /* __FIXEDMATH_H */
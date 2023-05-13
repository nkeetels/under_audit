#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "fixedmath.h"
#include "assets.h"

vector3_t blobs[4]; /* Z-component contains blob-radius */

EWRAM_DATA int lut_dist[256];

void effect_energyfield_init()
{
  for (int i = 0; i < 256; i++)
  { 
    uint8_t r0 = i * i * i >> 16;
    uint8_t g0 = i * i * i >> 16;
    uint8_t b0 = i;
    uint8_t r1 = i;
    uint8_t g1 = i;
    uint8_t b1 = i;

    int t = 192;

    uint8_t r = lerp(r0, r1, t);
    uint8_t g = lerp(g0, g1, t);
    uint8_t b = lerp(b0, b1, t);

    r >>= 3;
    g >>= 3;
    b >>= 3;

    ((unsigned short*)0x5000000)[i] = (b << 10) | (g << 5) | r;   
  }

  const uint16_t falloff = 6; /* 6 is nice for a 2d blob effect */

  for (int i = 1; i < 256; i++) {
    lut_dist[i] = min(92, div(190000*2, 1 + (i << falloff)));
  }
  lut_dist[0] = 255;
  lut_dist[255] = 0;
}

void effect_energyfield_destroy()
{
}

void IWRAM_CODE render_blobs(uint16_t *target)
{
  uint16_t sum = 0;
  uint16_t x, y, offset;
  uint16_t margin = 50;

  #define BLOPPER {\
    sum = lut_dist[(pow(blobs[0].x - x) << (1 + blobs[0].z)) + (pow(blobs[0].y - y) << (blobs[0].z))] +\
          lut_dist[(pow(blobs[1].x - x) << (1 + blobs[1].z)) + (pow(blobs[1].y - y) << (blobs[1].z))] +\
          lut_dist[(pow(blobs[2].x - x) << (1 + blobs[2].z)) + (pow(blobs[2].y - y) << (blobs[2].z))] +\
          lut_dist[(pow(blobs[3].x - x) << (1 + blobs[3].z)) + (pow(blobs[3].y - y) << (blobs[3].z))];\
    if (sum > 255) sum = 255;\
    target[offset + x] = (sum << 8) | sum;\
    target[offset + 120 + x] = (sum << 8) | sum;\
    ++x;\
  }

  for (y = margin; y < VIEWPORT_HEIGHT-margin; y+=2) {
    x = (margin>>1);
    offset = y * 120;
    while (x < (VIEWPORT_WIDTH >> 1)-(margin>>1)) {
      BLOPPER;
      BLOPPER;
      BLOPPER;
      BLOPPER;
      BLOPPER;
      BLOPPER;
      BLOPPER;
      BLOPPER;
    }
  }
}

void effect_energyfield_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  blobs[0].z = 0;
  blobs[1].z = 1;
  blobs[2].z = 2;
  blobs[3].z = 3;

  blobs[0].x = 60 + (cos(frame) >> 4); 
  blobs[0].y = 80 + (sin(frame<<1) >> 4);
  blobs[1].x = 60 + (-cos(((frame<<2) + 20)) >> 5); 
  blobs[1].y = 80 + (sin(frame<<1) >> 5);
  blobs[2].x = 60 + (cos(((frame<<1) + 88)) >> 5); 
  blobs[2].y = 80 + (-sin(frame<<1) >> 5);
  blobs[3].x = 60 + (sin(((frame<<1) + 88)) >> 4); 
  blobs[3].y = 80 + (-cos(frame<<1) >> 4);

  render_blobs(target);
}
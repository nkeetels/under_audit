#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "fixedmath.h"
#include "assets.h"

extern uint16_t *backbuffer;

uint16_t angle = 0;

void effect_radialblur_init()
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
}

void effect_radialblur_destroy()
{
  
}

void effect_radialblur_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  zoom_blur(target);

  uint16_t time = frame << 3;

  int16_t x = 60-16 + (SinLUT[((time >> 1) + 64) & SINLUT_SIZE] >> 4);
  int16_t y = 60-16 + (SinLUT[(time) & SINLUT_SIZE] >> 4);
  uint16_t offset = 0;
  for (int j = 0; j < 64; j++) {
    for (int i = 0; i < 32; i++) {
      uint8_t color = (spr_logoBitmap[offset++]) << 3;
      if (color > 0)
        target[(j + y) * 120 + i + x] = (color << 8) | color;
    }
  }

  x = 60-16 - (SinLUT[((time >> 1) + 64) & SINLUT_SIZE] >> 4);
  y = 60-16 - (SinLUT[(time) & SINLUT_SIZE] >> 4);
  offset = 0;
  for (int j = 0; j < 64; j++) {
    for (int i = 0; i < 32; i++) {
      uint8_t color = (spr_logoBitmap[offset++]) << 3;
      if (color > 0)
        target[(j + y) * 120 + i + x] = (color << 8) | color;
    }
  }

}

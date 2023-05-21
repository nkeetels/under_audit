#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "fixedmath.h"
#include "assets.h"

extern const unsigned short threestateBitmap[2048];
extern const unsigned short aardbeiBitmap[2048];
extern const unsigned short desireBitmap[2048];
extern const unsigned short eopBitmap[2048];
extern const unsigned short guidelineBitmap[2048];
extern const unsigned short kaleidoBitmap[2048];
extern const unsigned short poobrainBitmap[2048];
extern const unsigned short tpbBitmap[2048];

#define NUM_GREETINGS 8
const static unsigned short *greetings_bitmaps[] = {
  threestateBitmap,
  aardbeiBitmap,
  desireBitmap,
  eopBitmap,
  guidelineBitmap,
  kaleidoBitmap,
  poobrainBitmap,
  tpbBitmap
};

uint8_t current_greet = 0;

void effect_greetings_init()
{
  for (int i = 0; i < 256; i++)
  { 
    uint8_t r0 = i * i * i >> 16;
    uint8_t g0 = i * i * i >> 16;
    uint8_t b0 = i;
    uint8_t r1 = i;
    uint8_t g1 = i;
    uint8_t b1 = i;

    int t = 300;

    uint8_t r = lerp(r0, r1, t);
    uint8_t g = lerp(g0, g1, t);
    uint8_t b = lerp(b0, b1, t);

    r >>= 3;
    g >>= 3;
    b >>= 3;

    generated_palette[i] = (b << 10) | (g << 5) | r;   
  }

  set_palette(generated_palette);
}

void effect_greetings_destroy()
{
  
}

void IWRAM_CODE software_sprite(uint16_t *target, uint16_t *img, int x, int y)
{
  uint16_t offset = 0;
  x -= 16;
  y -= 32;

  for (int j = 0; j < 32; j++) {
    for (int i = 0; i < 64; i++) {
      uint8_t color = (img[offset++]) << 3;
      if (color > 0)
        target[(j + y) * 120 + i + x] = (color << 8) | color;
    }
  }
}

void effect_greetings_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  zoom_blur(target);

  if ((sync & 1) == 0) {
    int x = (rand() & 63)-32;
    int y = (rand() & 63)-32;
    software_sprite(target, greetings_bitmaps[(current_greet++) & 7], x+60-16, y+80+16); 
  }
}

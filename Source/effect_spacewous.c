#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "fixedmath.h"
#include "assets.h"

#if 1
int fade_to_white = 0;
#endif

static uint16_t spacewous_frame = 0;

void effect_spacewous_init()
{
  set_palette(tex_godwatbotPal);

  for (int i = 1; i < 128; ++i) {
    ((uint16_t*)0x5000200)[i] = (31 << 10) | (31 << 5) | 31;
  }
}

void effect_spacewous_destroy()
{
  clear_sprites();
  commit_sprites();
}

void effect_spacewous_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  frame = spacewous_frame++;

  int fade_to_black = min(frame << 2, 255);
  fade_palette((unsigned short*)0x5000000, tex_godwatbotPal, 0, 0, 0, 255 - fade_to_black);

  if (frame > 96*2+32) {
    fade_to_white += 14;
    fade_palette((unsigned short*)0x5000000, tex_godwatbotPal, 31, 31, 31, min(fade_to_white, 255));
  }


	int time = frame << 2;

  int x = (cos(time >> 1) >> 1);
  int y = (sin(time) >> 1);

	int t = ((((time + x) & 0xFF) << 8) | ((time + y) & 0xFF)) & 0x7fff;
  move_table(target, (uint8_t*)tex_godwatbotBitmap, (uint16_t*)lut_sphere, 40+(x>>2), 80+(y>>2)-20, -t>>1);
}

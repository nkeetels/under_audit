#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "fixedmath.h"
#include "assets.h"

void effect_ball_init()
{
  set_palette(tex_godwatbotPal);
}

void effect_ball_destroy()
{
  
}

void effect_ball_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
	uint16_t time = frame << 2;

  int16_t x = (SinLUT[((time >> 1) + 64) & SINLUT_SIZE] >> 1);
  int16_t y = (SinLUT[(time) & SINLUT_SIZE] >> 1);

	int16_t t = ((((time + x) & 0xFF) << 8) | ((time + y) & 0xFF)) & 0x7fff;
  move_table(target, (uint8_t*)tex_godwatbotBitmap, (uint16_t*)lut_ball, 40+(x>>2), 80+(y>>2)-20, -t>>1);
}

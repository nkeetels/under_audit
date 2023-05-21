#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "fixedmath.h"
#include "assets.h"
#include "sync.h"

int lut_offset = 0;
int fade_timer = 0;

void effect_ball_init()
{
  set_palette(tex_godwatbotPal);
  clear_sprites();
  commit_sprites();
}

void effect_ball_destroy()
{
  
}

void IWRAM_CODE effect_ball_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
	uint16_t time = lut_offset;//frame << 2;
  lut_offset += 7;

  if (sync_trigger(kick_pattern, 10, sync+1)) {
    lut_offset += 13;//(rand() & 31);
    fade_timer = 255;
  }

  fade_timer -= 64;
  fade_timer = max(0, fade_timer);
  fade_palette((unsigned short*)0x5000000, tex_godwatbotPal, 31, 31, 31, fade_timer);

  int16_t x = (SinLUT[((time >> 1) + 50) & SINLUT_SIZE] >> 1);
  int16_t y = (SinLUT[(time) & SINLUT_SIZE] >> 1);

	int16_t t = ((((time + x) & 0xFF) << 8) | ((time + y) & 0xFF)) & 0x7fff;
  move_table(target, (uint8_t*)tex_godwatbotBitmap, (uint16_t*)lut_ball, 40+(x>>2), 80+(y>>2)-20, -t>>1);

}

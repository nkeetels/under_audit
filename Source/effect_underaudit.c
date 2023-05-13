#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "fixedmath.h"
#include "assets.h"

#if 0
int fade_to_white = 0;
#endif

void effect_underaudit_init()
{
  for (int i = 1; i < 255; ++i) {
    ((uint16_t*)0x5000000)[i] = (i >> 4 << 10) | (i >> 4 << 5) | i >> 4;
  }

	clear_sprites();

  int sprite_pos = 22;

	set_sprite(0, sprite_pos, 80-32, 64, 0, 1, spr_under_audit_outline1Tiles);
	set_sprite(1, sprite_pos + 64, 80-32, 64, 0, 1, spr_under_audit_outline2Tiles);
	set_sprite(2, sprite_pos + 128, 80-32, 64, 0, 1, spr_under_audit_outline3Tiles);

  set_sprites_transparency(16);

  commit_sprites();

  clear_screen(0);
}


void effect_underaudit_destroy()
{
  rotate_background(0); 

  clear_sprites();
  commit_sprites();
}

void animate_sprite_palette(uint32_t frame)
{
  frame <<= 0;

  fast_set((uint16_t*)0x5000200, 0x0000, 256);

  for (int i = 1; i < frame && i < 128; ++i) {
    ((uint16_t*)0x5000200)[i] = (31 << 10) | (31 << 5) | 31;
  }
}

void effect_underaudit_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  rotate_background(-frame);

  fast_copy(target, tex_logo_fullscreenBitmap, VIEWPORT_WIDTH * VIEWPORT_HEIGHT >> 1);

  if (frame > 64) {
    animate_sprite_palette(frame - 64);
  }

#if 0 // fade in
  int fade_to_black = min(frame << 2, 255);
  fade_palette((unsigned short*)0x5000000, tex_godwatbotPal, 0, 0, 0, 255 - fade_to_black);

  if (frame > 96) {
    fade_to_white += 14;
    fade_palette((unsigned short*)0x5000000, tex_godwatbotPal, 31, 31, 31, min(fade_to_white, 255));
  }

	int time = frame << 2;

  int x = (cos(time >> 1) >> 1);
  int y = (sin(time) >> 1);

	int t = ((((time + x) & 0xFF) << 8) | ((time + y) & 0xFF)) & 0x7fff;
  move_table(target, (uint8_t*)tex_godwatbotBitmap, (uint16_t*)lut_sphere, 40+(x>>2), 80+(y>>2)-20, -t>>1);
#endif
}

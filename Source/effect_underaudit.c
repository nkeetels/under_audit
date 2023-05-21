#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "fixedmath.h"
#include "assets.h"

#if 0
int fade_to_white = 0;
#endif

uint8_t logo_visible = 0;
uint16_t fade_to_black_logo = 0;

void effect_underaudit_init()
{
  for (int i = 1; i < 255; ++i) {
    generated_palette[i] = (i >> 4 << 10) | (i >> 4 << 5) | i >> 4;
  }
  generated_palette[0] = 0x0000;

  set_palette(generated_palette);

	clear_sprites();

  commit_sprites();

  clear_screen(255);
}


void effect_underaudit_destroy()
{
  rotate_background(0, 255); 
#if 0
  clear_sprites();
  commit_sprites();
#endif
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
  rotate_background(-frame, 512-(frame << 1));

  fast_copy(target, tex_logo_fullscreenBitmap, VIEWPORT_WIDTH * VIEWPORT_HEIGHT >> 1);

  if (!logo_visible && sync == 32+4) {
    int sprite_pos = 22;

    set_sprite(0, sprite_pos, 80-32, 64, 0, 1, spr_under_audit_outline1Tiles);
    set_sprite(1, sprite_pos + 64, 80-32, 64, 0, 1, spr_under_audit_outline2Tiles);
    set_sprite(2, sprite_pos + 128, 80-32, 64, 0, 1, spr_under_audit_outline3Tiles);

    set_sprites_transparency(16);

    commit_sprites();

    logo_visible = 1;
  }
  
  if (logo_visible > 0) {
    animate_sprite_palette(logo_visible);
    logo_visible++;
  }

  add_palette((uint16_t*)0x5000000, generated_palette, frame % (rand() % 5));

  if (sync > 48) {
    fade_to_black_logo += 4;
    fade_palette((unsigned short*)0x5000000, generated_palette, 0, 0, 0, min(fade_to_black_logo, 255));
  }
  else {
    set_mosaic(164, 0, 0, 0);
  }
}

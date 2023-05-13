#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "fixedmath.h"
#include "assets.h"

uint8_t credits_sprite_set = 0;
uint8_t credits_sprite_pos;
uint8_t credits_sprite_start_pos = 120-32;

void effect_credits_init()
{
  set_sprite_palette(spr_credits_Pal);

	clear_sprites();

  credits_sprite_pos = credits_sprite_start_pos;

	set_sprite(0, credits_sprite_pos + 00, 80-32, 64, 0, 0, spr_ravity_1Tiles);
	set_sprite(1, credits_sprite_pos + 64, 80-32, 64, 0, 0, spr_ravity_2Tiles);

  set_sprites_transparency(16);

  commit_sprites();

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

    ((unsigned short*)0x5000000)[i] = (b << 10) | (g << 5) | r;   
  }

  /* of phong-achtig palette? */
  set_palette(golden_gradient);
}

void effect_credits_destroy()
{
  clear_sprites();
  commit_sprites();
  clear_screen(0);
}

void effect_credits_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  zoom_blur(target);


  if ((frame % 25) == 0) {

  	clear_sprites();

    credits_sprite_pos = credits_sprite_start_pos;

    uint16_t *spr0 = 0, *spr1 = 0;
    switch (credits_sprite_set) {
      case 0:
        spr0 = (uint16_t*)spr_ravity_1Tiles;
        spr1 = (uint16_t*)spr_ravity_2Tiles;
        break;
      case 1:
        spr0 = (uint16_t*)spr_crash_1Tiles;
        spr1 = (uint16_t*)spr_crash_2Tiles;
        break;
      case 2: 
        spr0 = (uint16_t*)spr_kit11_1Tiles;
        spr1 = (uint16_t*)spr_kit11_2Tiles;
        break;
    }
	  set_sprite(0, credits_sprite_pos + 00, 80-32, 64, 0, 0, spr0);
	  set_sprite(1, credits_sprite_pos + 64, 80-32, 64, 0, 0, spr1);

    ++credits_sprite_set;
  }

  set_sprite_position(0, --credits_sprite_pos + 00, 80-32);
  set_sprite_position(1, --credits_sprite_pos + 64, 80-32);
 
  commit_sprites();

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

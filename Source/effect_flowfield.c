#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "fixedmath.h"
#include "sprites.h"
#include "assets.h"


void effect_flowfield_init()
{
  set_sprite_palette(spr_mindshifterPal);

	clear_sprites();

  int sprite_pos = 22;

	set_sprite(0, sprite_pos, 80-32, 64, 0, 0, spr_mindshifter1Tiles);
	set_sprite(1, sprite_pos + 64, 80-32, 64, 0, 0, spr_mindshifter2Tiles);
	set_sprite(2, sprite_pos + 128, 80-32, 64, 0, 0, spr_mindshifter3Tiles);

  set_sprites_transparency(16);

  commit_sprites();

  clear_screen(0);

}

void effect_flowfield_destroy()
{
  set_mosaic(0, 0, 0, 0);
}


void effect_flowfield_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  int16_t scale = 15-(frame>>1);
  if (scale >= 0) {
    set_mosaic(0, 0, scale, scale);
  }
}

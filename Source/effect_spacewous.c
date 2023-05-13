#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "fixedmath.h"
#include "assets.h"

#if 1
int fade_to_white = 0;
#endif

void effect_spacewous_init()
{
  set_palette(tex_godwatbotPal);
return;
	clear_sprites();

  set_sprite_palette(spr_solarLogoPal);
	set_sprite(0, (VIEWPORT_WIDTH>>1)-32, (VIEWPORT_HEIGHT>>1)-32, 64, 1, 1, spr_solarLogoTiles);


  set_sprites_transparency(16);

  commit_sprites();
}

void effect_spacewous_destroy()
{
  
}

void effect_spacewous_update(uint16_t *target, uint32_t frame, uint16_t sync)
{

#if 1
  int fade_to_black = min(frame << 2, 255);
  fade_palette((unsigned short*)0x5000000, tex_godwatbotPal, 0, 0, 0, 255 - fade_to_black);

  if (frame > 96) {
    fade_to_white += 14;
    fade_palette((unsigned short*)0x5000000, tex_godwatbotPal, 31, 31, 31, min(fade_to_white, 255));
    set_sprites_transparency(16 - (fade_to_white >> 4));
  }
#endif
	int time = frame << 2;

  int x = (cos(time >> 1) >> 1);
  int y = (sin(time) >> 1);

	int t = ((((time + x) & 0xFF) << 8) | ((time + y) & 0xFF)) & 0x7fff;
  move_table(target, (uint8_t*)tex_godwatbotBitmap, (uint16_t*)lut_sphere, 40+(x>>2), 80+(y>>2)-20, -t>>1);


return;
  int zoom = frame;
  rotate_sprite(0, zoom<<1, 340-zoom, 340-zoom);
  set_sprites_transparency(zoom);
  commit_sprites();

}

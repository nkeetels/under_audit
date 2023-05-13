#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "fixedmath.h"
#include "meshgenerator.h"
#include "assets.h"

#define WIP
#define WOBBLY

void generate_palette()
{
  for (int i = 0; i < 256; i++)
  { 
    uint8_t r0 = i * i * i >> 16;
    uint8_t g0 = i * i * i >> 16;
    uint8_t b0 = i;
    uint8_t r1 = i;
    uint8_t g1 = i;
    uint8_t b1 = i;

    int t = 220;

    uint8_t r = lerp(r0, r1, t);
    uint8_t g = lerp(g0, g1, t);
    uint8_t b = lerp(b0, b1, t);

    r >>= 4;
    g >>= 4;
    b >>= 4;

    ((unsigned short*)0x5000000)[i] = (b << 10) | (g << 5) | r;   
  }
}

void effect_spheredistort_init()
{
  generate_palette();

	clear_sprites();

  set_sprite_palette(solar_lPal);

	set_sprite(0, 120-32, 80-32, 64, 0, 0, solar_lTiles);
	set_sprite(1, 120+32, 80-32, 64, 0, 0, solar_rTiles);

  set_sprites_transparency(16);

  commit_sprites();

  current_model.flags = MODEL_BACKFACECULLING | MODEL_SORTFACES;
  current_model.position.x = 0;
  current_model.position.y = 0;
  current_model.position.z = -200;
}

void effect_spheredistort_destroy()
{
}

void effect_spheredistort_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  clear_screen(0);

  int radius, i, x, y;
  int step = 360 >> 6;
  int c = 4;
  do {
    radius = 30 + ((20 * sin(((frame << 2) + (c << 5))) >> 6));
    i = 64;
    do {
      x = (radius * cos(i * step)) >> 8;
      y = (radius * sin(i * step)) >> 8;

      put_pixel(x + 120, y + 80, 200);
    } while (--i);
  } while (--c);

  subdivide_icosahedron(frame << 2, MESH_VERTEXDISPLACE, 0);

  render(&current_model);

  bloom(target, 6, 15);
}

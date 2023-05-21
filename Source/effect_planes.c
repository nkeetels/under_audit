#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "fixedmath.h"
#include "assets.h"
#include "sync.h"

uint8_t credits_sprite_set = 0;
uint8_t credits_sprite_pos;
uint8_t credits_sprite_start_pos = 120-32;

int mosaic_scale = 0;

void effect_planes_init()
{
  clear_buffers(255);

  int i;
  for (i = 0; i < 256; i++)
  {
    int r = (metalPal[i] & 0x7C00) >> 10;
    int g = (metalPal[i] & 0x3E0) >> 5;
    int b = (metalPal[i] & 0x1F);

    int r1 = (golden_gradient[i] & 0x7C00) >> 10;
    int g1 = (golden_gradient[i] & 0x3E0) >> 5;
    int b1 = (golden_gradient[i] & 0x1F);

    r = (r * 24 + r1 * 8) >> 5;
    g = (g * 24 + g1 * 8) >> 5;   
    b = (b * 24 + b1 * 8) >> 5;

    r = r * r;
    g = g * g;
    b = b * b;

    r >>= 5;
    g >>= 5;
    b >>= 5;


    generated_palette[i] = (r << 10) | (r << 5) | g;
  }  
  set_palette(generated_palette);
  set_sprite_palette(spr_credits_Pal);



  scale_palette((unsigned short*)0x5000000, generated_palette, 5);

	clear_sprites();

  int credits_sprite_pos = 50;

	set_sprite(0, credits_sprite_pos + 00, 80-32, 64, 0, 0, spr_ravity_1Tiles);
	set_sprite(1, credits_sprite_pos + 64, 80-32, 64, 0, 0, spr_ravity_2Tiles);

  set_sprites_transparency(16);

  commit_sprites();

}

void effect_planes_destroy()
{
  
}

void IWRAM_CODE raytrace_planes(uint16_t *target, uint16_t frame)
{
  matrix3x3_t m;

  rotate(m, sin(frame << 1) >> 4, cos(frame<<1) >> 3, -sin((frame+40) << 1) >> 1);

  int u, v;
  int i, j;
  int offset = 0, voffset = 0;
  int ox, oy, oz;
  int dx, dy, dz;
  int nx, ny, nz;

  int ix, iz, t, ilen;

  ox = 0;
  oy = -sin(frame) << 2;
  oz = frame * 10;

  j = 0;

  do 
  {
    i = 0;
    do
    {                                        
      dx = (((i << 1) - 120) << 1);
      dy = ((j - 80) << 1);
      dz = 256;

      nx = (m[0][0] * dx + m[0][1] * dy + m[0][2] * dz);
      ny = (m[1][0] * dx + m[1][1] * dy + m[1][2] * dz);
      nz = (m[2][0] * dx + m[2][1] * dy + m[2][2] * dz);

      ilen = sqrt(nx * nx + ny * ny + nz * nz) >> 8;

      nx = div(nx, ilen);
      ny = div(ny, ilen);
      nz = div(nz, ilen);

      dx = (nx * ilen) >> 8;
      dy = (ny * ilen) >> 8;
      dz = (nz * ilen) >> 8;

      if (dy == 0) {
        t = 65535;
      }
      else {        
        t = div((sign(dy) << 13) - oy, dy);
      }

      ix = ox + ((int)(dx * t) >> 7);
      iz = oz + ((int)(dz * t) >> 7);

      u = (int)(abs(ix)) + 64;
      v = (int)(abs(iz)) + 64;

      t >>= 6;
      t = min(t, 7);

      uvtable[offset++] = u;
      uvtable[offset++] = v;
      uvtable[offset++] = t << 5;

      voffset++;

      i += 16;
    } while (i < 120);
    voffset+=120;
    j += 16;
  } while (j < 160);
}

void effect_planes_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  raytrace_planes(target, frame);

  uv_table(target, (uint8_t*)metalBitmap, uvtable, frame);

 
  if (((frame+1) % 25) == 0) {
    ++credits_sprite_set;
  	clear_sprites();

    credits_sprite_pos = credits_sprite_start_pos;

    uint16_t *spr0 = 0, *spr1 = 0;
    uint8_t show_sprites = 1;
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
      default:
        show_sprites = 0; 
        clear_sprites();
        break;

    }

    if (show_sprites) {
	    set_sprite(0, credits_sprite_pos + 00, 80-32, 64, 0, 0, spr0);
	    set_sprite(1, credits_sprite_pos + 64, 80-32, 64, 0, 0, spr1);
    }
  }


  if (sync_trigger(kick_pattern, 10, (sync))) {
    mosaic_scale = 24;
  }

  mosaic_scale = max(0, mosaic_scale);
  set_mosaic(0, 0, mosaic_scale>>1, mosaic_scale>>1);
  mosaic_scale-=6;

}

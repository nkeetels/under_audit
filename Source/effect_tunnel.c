#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "fixedmath.h"
#include "assets.h"

void effect_tunnel_init()
{
  clear_buffers(0);

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


    generated_palette[i] = (b << 10) | (r << 5) | r;
  }  
  set_palette(generated_palette);
  set_sprite_palette(spr_credits_Pal);
}

void effect_tunnel_destroy()
{
}

void IWRAM_CODE raytrace_tunnel(uint16_t *target, uint16_t frame)
{
  matrix3x3_t m;

  rotate(m, sin(frame) >> 4, cos(frame<<1) >> 3, -sin((frame) << 1) >> 1);

  int u, v;
  int i, j;
  int offset = 0, voffset = 0;
  int ox, oy, oz;
  int dx, dy, dz;
  int nx, ny, nz;

  int ix, iy, iz, t, ilen;
  int a, b, c, delta, t1, t2;

  ox = 0;
  oy = 0;
  oz = 0;          

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

      a = dx * dx + dy * dy;
      b = 2 * (ox * dx + oy * dy);
      c = ox * ox + oy * oy - 64 * 64;

      delta = _sqrt(b * b - 4 * a * c);
      t1 = _div((-b + delta) << 8, 2 * a);
      t2 = _div((-b - delta) << 8, 2 * a);

      t = min(t1, t2);

      ix = (ox + (dx * t)) >> 8;
      iy = (oy + (dy * t)) >> 8;
      iz = (oz + (dz * t)) >> 8;

      u = (int)(iz);
      v = (int)((abs((int)(_atan2(iy, ix)))) >> 5);

      t = (max(t1, t2)) >> 2;

      if (t > 255) t = 255;

      uvtable[offset++] = u;
      uvtable[offset++] = v;
      uvtable[offset++] = t;

      voffset++;

      i += 16;
    } while (i < 120);
    voffset+=120;
    j += 16;
  } while (j < 160);
}

void effect_tunnel_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  raytrace_tunnel(target, frame);

  uv_table(target, (uint8_t*)metalBitmap, uvtable, frame);
/*
  int inversion = 128 + (cos(frame << 2) >> 2);

  invert_palette((unsigned short*)0x5000000, generated_palette, inversion);
*/
}

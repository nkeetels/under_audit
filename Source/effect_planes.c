#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "fixedmath.h"
#include "assets.h"

void effect_planes_init()
{
  clear_screen(0);

  int i;
  for (i = 0; i < 256; i++)
  {
    uint8_t r = (metalPal[i] & 0x7C00) >> 10;
    uint8_t g = (metalPal[i] & 0x3E0) >> 5;
    //uint8_t b = (metalPal[i] & 0x1F);

    ((unsigned short*)0x5000000)[i] = (r << 10) | (r << 5) | g;
  }  
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
}

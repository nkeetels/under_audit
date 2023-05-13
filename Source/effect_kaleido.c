#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "fixedmath.h"
#include "sprites.h"
#include "assets.h"

#define KALEIDO_SIDES 10

void effect_kaleido_init()
{
  int i;
  int offset = 255>>1;
  for (i = 0; i < 256; i++)
  {
    uint8_t r = (tex_godwatbotPal[i] & 0x7C00) >> 10;
    uint8_t g = (tex_godwatbotPal[i] & 0x3E0) >> 5;
    uint8_t b = (tex_godwatbotPal[i] & 0x1F);

    r += (SinLUT[offset] >> 2);
    g += (SinLUT[offset] >> 2);
    b += (SinLUT[offset] >> 2);

    ((unsigned short*)0x5000000)[i] = (b << 10) | (g << 5) | r;
  }

  set_palette(golden_gradient);

  current_model.triangles = &triangle_list[0];
  for (int i = 0; i < KALEIDO_SIDES; i ++)
  {
    triangle_list[i].next = &triangle_list[i+1];
  }
  current_model.triangles[KALEIDO_SIDES-1].next = 0;
  current_model.flags = MODEL_TEXTURED;

  current_model.position.x = 0;
  current_model.position.y = 0;
  current_model.position.z = 0;

  camera_position.x = 0;
  camera_position.y = 0;
  camera_position.z = 64;
}

void effect_kaleido_destroy()
{
}

void UpdateKaleidoscope(triangle_t f[], int sides, int time)
{
  int x, y;
  float step;

  const int angle = time;
  const int zoom = 16;

  vector2_t center;

  center.x = 32 + ((cos((time << 2))) >> 1);
  center.y = 64 + ((sin(((time << 1) - 85))) >> 1);

  vector2_t tc0, tc1, tc2;

  tc0.x = center.x + ((zoom * cos((angle - 85))) >> 6);
  tc0.y = center.y + ((zoom * sin((angle - 85))) >> 6);
  tc1.x = center.x + ((zoom * cos(angle)) >> 6);
  tc1.y = center.y + ((zoom * sin(angle)) >> 6);
  tc2.x = center.x + ((zoom * cos((angle + 85))) >> 6);
  tc2.y = center.y + ((zoom * sin((angle + 85))) >> 6);

  for (int i = 0; i < sides; i++)
  {
    x = 0;
    y = 0;

    f[i].vertices[0].position.x = x;
    f[i].vertices[0].position.y = y;
    f[i].vertices[0].position.z = 1;

    step = (float)sides / i;
    f[i].vertices[1].position.x = SinLUT[(int)(64 + (SINLUT_SIZE+1) / step) & SINLUT_SIZE] >> 1;
    f[i].vertices[1].position.y = (2 * SinLUT[(int)((SINLUT_SIZE+1) / step) & SINLUT_SIZE]) >> 2;
    f[i].vertices[1].position.z = 1;

			
		step = (float)sides / (i + 1);
    f[i].vertices[2].position.x = SinLUT[(int)(64 + (SINLUT_SIZE+1) / step) & SINLUT_SIZE] >> 1;
    f[i].vertices[2].position.y = (2 * SinLUT[(int)((SINLUT_SIZE+1) / step) & SINLUT_SIZE]) >> 2;
    f[i].vertices[2].position.z = 1;

    f[i].vertices[0].u = tc0.x;
    f[i].vertices[0].v = tc0.y;
    f[i].vertices[1].u = ((i & 1) == 0) ? tc1.x : tc2.x;
    f[i].vertices[1].v = ((i & 1) == 0) ? tc1.y : tc2.y;
    f[i].vertices[2].u = ((i & 1) == 0) ? tc2.x : tc1.x;
    f[i].vertices[2].v = ((i & 1) == 0) ? tc2.y : tc1.y;    

    f[i].texture_index = 0;
  }
}


void effect_kaleido_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  fast_set(target, 0x0000, (VIEWPORT_WIDTH * VIEWPORT_HEIGHT) >> 1);

  UpdateKaleidoscope(triangle_list, KALEIDO_SIDES, frame);

  for (int i = 0; i < KALEIDO_SIDES; i++) {
    triangle_t t = triangle_list[i];

    t.vertices[0].position.x += 120;
    t.vertices[1].position.x += 120;
    t.vertices[2].position.x += 120;

    t.vertices[0].position.y += 80;
    t.vertices[1].position.y += 80;
    t.vertices[2].position.y += 80;

    texture_triangle(&t);
  }
}

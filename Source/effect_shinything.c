#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "fixedmath.h"
#include "meshgenerator.h"
#include "assets.h"

extern triangle_t transformed_list[MAX_VERTICES];

void effect_shinything_init()
{
  set_palette(tex_litsphere2Pal);

  current_model.flags = MODEL_BACKFACECULLING | MODEL_SORTFACES | MODEL_ENVMAPPED | MODEL_TEXTURED;
  current_model.position.z = -200;

  //set_overlay(greetings1Tiles, greetings2Tiles, greetings3Tiles, greetings4Tiles, greetings1Pal);
}

void effect_shinything_destroy()
{
  clear_sprites();
  commit_sprites();
}

void effect_shinything_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  clear_screen(0);
#if 0  
  current_model.euler_angles.x = frame >> 1;
  current_model.euler_angles.z = frame << 1;
#endif
  subdivide_icosahedron(frame << 2, MESH_VERTEXDISPLACE, 7);

  render(&current_model);
}

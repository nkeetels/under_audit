#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "meshgenerator.h"
#include "fixedmath.h"
#include "assets.h"

#define DEPTH 129
#define FUDGE 68

const int16_t geo_plane[] = { -64, -64, 1, 127, 63, 
                              64, -64, 1, 0, 63, 
                              -64, 64, 1, 127, 0, 
                              -64, 64, 1, 127, 0, 
                              64, -64, 1, 0, 63, 
                              64, 64, 1, 0, 0 };

const int16_t geo_ceiing[] = { -64, 64, 1, 0, 0, 64, 64, 1, 128, 0, -64, FUDGE, DEPTH, 0, 64, 
                              -64, FUDGE,DEPTH, 0, 64, 64, 64, 1, 128, 0, 64, FUDGE, DEPTH, 128, 64 };
/*
const int16_t geo_near_cealing[] = { -64, -FUDGE/2, 1, 0, 0, 64, -FUDGE/2, 1, 128, 0, -64, 64/2, DEPTH/2, 0, 64, 
                                     -64, 64/2,DEPTH/2, 0, 64, 64, -FUDGE/2, 1, 128, 0, 64, 64/2, DEPTH/2, 128, 64 };
*/
const int16_t geo_near_cealing[] = { -64, -FUDGE/2, 1, 127, 63, 
                                      64, -FUDGE/2, 1, 0, 63, 
                                      -64, 64/2, DEPTH/2, 127, 0, 
                                     -64, 64/2, DEPTH/2, 127, 0, 
                                     64, -FUDGE/2, 1, 0, 63, 
                                     64, 64/2, DEPTH/2, 0, 0 };

const int16_t geo_floor[] = { -64, 64, 1, 0, 0, 64, 64, 1, 128, 0, -64, 64, DEPTH, 0, 64, 
                              -64, 64,DEPTH, 0, 64, 64, 64, 1, 128, 0, 64, 64, DEPTH, 128, 64 };


void add_segment(int16_t *geometry, int16_t offset_x, int16_t offset_y, int16_t offset_z, uint8_t scale, uint8_t texture_index)
{
  triangle_list[triangle_count].vertices[0].position.x = (*geometry++) * scale + offset_x;
  triangle_list[triangle_count].vertices[0].position.y = (*geometry++) * scale + offset_y;
  triangle_list[triangle_count].vertices[0].position.z = (*geometry++) * scale + offset_z;
  triangle_list[triangle_count].vertices[0].u = *geometry++;
  triangle_list[triangle_count].vertices[0].v = *geometry++;
  triangle_list[triangle_count].vertices[1].position.x = (*geometry++) * scale + offset_x;
  triangle_list[triangle_count].vertices[1].position.y = (*geometry++) * scale + offset_y;
  triangle_list[triangle_count].vertices[1].position.z = (*geometry++) * scale + offset_z;
  triangle_list[triangle_count].vertices[1].u = *geometry++;
  triangle_list[triangle_count].vertices[1].v = *geometry++;
  triangle_list[triangle_count].vertices[2].position.x = (*geometry++) * scale + offset_x;
  triangle_list[triangle_count].vertices[2].position.y = (*geometry++) * scale + offset_y;
  triangle_list[triangle_count].vertices[2].position.z = (*geometry++) * scale + offset_z;
  triangle_list[triangle_count].vertices[2].u = *geometry++;
  triangle_list[triangle_count].vertices[2].v = *geometry++;
  triangle_list[triangle_count].texture_index = texture_index;

  triangle_count++;

  triangle_list[triangle_count].vertices[0].position.x = (*geometry++) * scale + offset_x;
  triangle_list[triangle_count].vertices[0].position.y = (*geometry++) * scale + offset_y;
  triangle_list[triangle_count].vertices[0].position.z = (*geometry++) * scale + offset_z;
  triangle_list[triangle_count].vertices[0].u = *geometry++;
  triangle_list[triangle_count].vertices[0].v = *geometry++;
  triangle_list[triangle_count].vertices[1].position.x = (*geometry++) * scale + offset_x;
  triangle_list[triangle_count].vertices[1].position.y = (*geometry++) * scale + offset_y;
  triangle_list[triangle_count].vertices[1].position.z = (*geometry++) * scale + offset_z;
  triangle_list[triangle_count].vertices[1].u = *geometry++;
  triangle_list[triangle_count].vertices[1].v = *geometry++;
  triangle_list[triangle_count].vertices[2].position.x = (*geometry++) * scale + offset_x;
  triangle_list[triangle_count].vertices[2].position.y = (*geometry++) * scale + offset_y;
  triangle_list[triangle_count].vertices[2].position.z = (*geometry++) * scale + offset_z;
  triangle_list[triangle_count].vertices[2].u = *geometry++;
  triangle_list[triangle_count].vertices[2].v = *geometry++;
  triangle_list[triangle_count].texture_index = texture_index;

  triangle_count++;  
}

void effect_quake_init()
{
  new_mesh();

  set_palette(tex_subway1Pal);

  add_segment((int16_t*)geo_plane, -128*2, -128, 0, 2, 6);
  add_segment((int16_t*)geo_plane, 0, -129, 0, 2, 5);
  add_segment((int16_t*)geo_plane, 128*2, -128, 0, 2, 4);
/*
  add_segment((int16_t*)geo_plane, -128*2, 0, 0, 2, 0);
  add_segment((int16_t*)geo_plane, 0, 0, 0, 2, 1);
  add_segment((int16_t*)geo_plane, 128*2, 0, 0, 2, 2);
*/
  add_segment((int16_t*)geo_near_cealing, -128*2, 92-26, 0, 2, 3);
  add_segment((int16_t*)geo_near_cealing, 0, 92-26, 0, 2, 2);
  add_segment((int16_t*)geo_near_cealing, 128*2, 92-26, 0, 2, 1);
  /*
  add_segment((int16_t*)geo_ceiing, -128, 64, 64, 2, 0);
  add_segment((int16_t*)geo_ceiing, 0, 64, 64, 2, 1);
  add_segment((int16_t*)geo_ceiing, 128, 64, 64, 2, 2);
  */
  finalize_mesh(triangle_count);

  current_model.flags = MODEL_BACKFACECULLING | MODEL_SORTFACES | MODEL_TEXTURED;
  current_model.position.z = -421;
}

void effect_quake_destroy()
{
  
}

void effect_quake_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  clear_screen(0);

  current_model.position.x = sin(frame << 2);
/*
  current_model.euler_angles.x = frame;
  current_model.euler_angles.y = frame >> 1;
  current_model.euler_angles.z = frame >> 2;
*/
  render(&current_model);
}
 

#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "meshgenerator.h"
#include "fixedmath.h"
#include "assets.h"

#define DEPTH 129
#define FUDGE 68

uint16_t fade_to_black = 0;

const int16_t geo_plane2[] = { -64, -64, 1, 126, 63, 
                              64, -64, 1, 1, 63, 
                              -64, 64, 1, 126, 1, 
                              -64, 64, 1, 126, 1, 
                              64, -64, 1, 1, 63, 
                              64, 64, 1, 1, 1 };

const int16_t geo_pillar[] = { -4, -64, 1, 127, 63, 
                              4, -64, 1, 0, 63, 
                              -4, 64, 1, 127, 0, 
                              -4, 64, 1, 127, 0, 
                              4, -64, 1, 0, 63, 
                              4, 64, 1, 0, 0 };
      
const int16_t geo_destination_outline[] = { -64, -64, 1, 127, 63/3, 
                              64, -64, 1, 0, 63/3, 
                              -64, -64+32, 1, 127, 0, 
                              -64, -64+32, 1, 127, 0, 
                              64, -64, 1, 0, 63/3, 
                              64, -64+32, 1, 0, 0 };

const int16_t geo_destination[] = { -64, -64, 1, 127, 64/3+2, 
                              64, -64, 1, 0, 64/3+2, 
                              -64, -64+32, 1, 127, 15, 
                              -64, -64+32, 1, 127, 15, 
                              64, -64, 1, 0, 64/3+2, 
                              64, -64+32, 1, 0, 15 };

const int16_t geo_ceiing2[] = { -64, 64, 1, 0, 0, 64, 64, 1, 128, 0, -64, FUDGE, DEPTH, 0, 64, 
                              -64, FUDGE,DEPTH, 0, 64, 64, 64, 1, 128, 0, 64, FUDGE, DEPTH, 128, 64 };
/*
const int16_t geo_near_cealing[] = { -64, -FUDGE/2, 1, 0, 0, 64, -FUDGE/2, 1, 128, 0, -64, 64/2, DEPTH/2, 0, 64, 
                                     -64, 64/2,DEPTH/2, 0, 64, 64, -FUDGE/2, 1, 128, 0, 64, 64/2, DEPTH/2, 128, 64 };
*/
const int16_t geo_near_cealing2[] = { -64, -FUDGE/2, 1, 127, 63, 
                                      64, -FUDGE/2, 1, 0, 63, 
                                      -64, 64/2, DEPTH/2, 127, 0, 
                                     -64, 64/2, DEPTH/2, 127, 0, 
                                     64, -FUDGE/2, 1, 0, 63, 
                                     64, 64/2, DEPTH/2, 0, 0 };

const int16_t geo_floor2[] = { -64, 64, 1, 0, 0, 64, 64, 1, 128, 0, -64, 64, DEPTH, 0, 64, 
                              -64, 64,DEPTH, 0, 64, 64, 64, 1, 128, 0, 64, 64, DEPTH, 128, 64 };


void IWRAM_CODE add_segment2(int16_t *geometry, int16_t offset_x, int16_t offset_y, int16_t offset_z, uint8_t scale, uint8_t texture_index)
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

void effect_subway_init()
{
  new_mesh();

  set_palette(tex_subwayPal);

  // bijna grond
  //add_segment2((int16_t*)geo_plane2, 128*6, -128, 0, 2, 11);
  add_segment2((int16_t*)geo_plane2, 128*4, -128, 0, 2, 11);
  add_segment2((int16_t*)geo_plane2, -128*2, -128, 0, 2, 12);
  add_segment2((int16_t*)geo_plane2, 0, -129, 0, 2, 13);
  add_segment2((int16_t*)geo_plane2, 128*2, -128, 0, 2, 14);
  add_segment2((int16_t*)geo_plane2, -128*4, -128, 0, 2, 15);
  //add_segment2((int16_t*)geo_plane2, -128*6, -128, 0, 2, 11);
  
  // grond
  //add_segment2((int16_t*)geo_plane2, 128*6, -128-128-128, 0, 2, 12);
  add_segment2((int16_t*)geo_plane2, 128*4, -128-128-128, 0, 2, 16);
  add_segment2((int16_t*)geo_plane2, -128*2, -128-128-128, 0, 2, 17);
  add_segment2((int16_t*)geo_plane2, 0, -129-128-128, 0, 2, 18);
  add_segment2((int16_t*)geo_plane2, 128*2, -128-128-128, 0, 2, 19);
  add_segment2((int16_t*)geo_plane2, -128*4, -128-128-128, 0, 2, 20);
  //add_segment2((int16_t*)geo_plane2, -128*6, -128-128-128, 0, 2, 12);

  // bijna plafond
  //add_segment2((int16_t*)geo_near_cealing2, 128*6, 92-26, 0, 2, 12);
  add_segment2((int16_t*)geo_near_cealing2, 128*4, 92-26, 0, 2, 6);
  add_segment2((int16_t*)geo_near_cealing2, -128*2, 92-26, 0, 2, 7);
  add_segment2((int16_t*)geo_near_cealing2, 0, 92-26, 0, 2, 8);
  add_segment2((int16_t*)geo_near_cealing2, 128*2, 92-26, 0, 2, 9);
  add_segment2((int16_t*)geo_near_cealing2, -128*4, 92-26, 0, 2, 10);
  //add_segment2((int16_t*)geo_near_cealing2, -128*6, 92-26, 0, 2, 12);

  //plafond
 // add_segment2((int16_t*)geo_near_cealing2, 128*6, 119, 64, 2, 12);
  add_segment2((int16_t*)geo_near_cealing2, 128*4, 119, 64, 2, 1);
  add_segment2((int16_t*)geo_near_cealing2, -128*2, 119, 64, 2, 2);
  add_segment2((int16_t*)geo_near_cealing2, 0, 119, 64, 2, 3);
  add_segment2((int16_t*)geo_near_cealing2, 128*2, 119, 64, 2, 4);
  add_segment2((int16_t*)geo_near_cealing2, -128*4, 119, 64, 2, 5);
  //add_segment2((int16_t*)geo_near_cealing2, -128*6, 119, 64, 2, 12);
  /*
  add_segment2((int16_t*)geo_ceiing2, -128, 64-32, 32, 2, 12);
  add_segment2((int16_t*)geo_ceiing2, 0, 64-32, 32, 2, 12);
  add_segment2((int16_t*)geo_ceiing2, 128, 64-32, 32, 2, 12);
  */

  //add_segment2((int16_t*)geo_destination, -128*2, -128, 0, 2, 9);
  
  add_segment2((int16_t*)geo_pillar, -120, -128, 240, 2, 1);
  add_segment2((int16_t*)geo_pillar, 120, -128, 240, 2, 1);
  
  add_segment2((int16_t*)geo_pillar, -120, -128-64, 240, 2, 1);
  add_segment2((int16_t*)geo_pillar, 120, -128-64, 240, 2, 1);
  add_segment2((int16_t*)geo_destination_outline, 0, 64, 240, 2, 21);

  //add_segment2((int16_t*)geo_destination, 128*2, -128, 0, 2, 9);


  finalize_mesh(triangle_count);

  current_model.flags =/* MODEL_BACKFACECULLING | MODEL_SORTFACES | */MODEL_TEXTURED;
  current_model.position.z = -421;

  //set_overlay(greetings1Tiles, greetings2Tiles, greetings3Tiles, greetings4Tiles, greetings1Pal);
  fade_palette((unsigned short*)0x5000000, tex_subwayPal, 31, 31, 31, 255);
}

void effect_subway_destroy()
{
  finalize_mesh(0);
}

void effect_subway_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  if (frame > 60) {
    fade_to_black += 19;
    fade_palette((unsigned short*)0x5000000, tex_subwayPal, 0, 0, 0, min(fade_to_black, 255));
    //set_sprites_transparency(16 - (fade_to_black >> 4));
  }
  else {
    int fade_from_black = min(frame << 3, 255);
    fade_palette((unsigned short*)0x5000000, tex_subwayPal, 31, 31, 31, 255 - fade_from_black);
  }

  clear_screen(0);

  current_model.position.x = (sin((frame+32*4) << 1) >> 1);
  current_model.position.y = (sin((frame*2+32*4) << 1) >> 3);
  current_model.position.z  = -480 - 80;
  //current_model.euler_angles.x = frame;
 // current_model.euler_angles.y = frame >> 1;
  current_model.euler_angles.z = sin(((frame+32*4) << 1))>>4;

  render(&current_model);
}
 

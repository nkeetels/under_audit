#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "sprites.h"
#include "meshgenerator.h"
#include "fixedmath.h"
#include "assets.h"

const int8_t cube_vertices[] = {
  -1, -1, -1, -1, -1, 1, -1, 1, 1, 1, 1, -1, -1, -1, -1, -1, 1, -1, 1, -1, 1,
  -1, -1, -1, 1, -1, -1, 1, 1, -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1,
  -1, 1, -1, 1, -1, 1, -1, -1, 1, -1, -1, -1, -1, 1, 1, -1, -1, 1, 1, -1, 1,  
  1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1, 1, -1, 1, 1, 1, 1,1, 1, -1, -1,  
  1, -1, 1, 1, 1, -1, 1, -1, -1, 1, 1, 1, 1, 1, -1, 1, 1, 1, -1, 1
};  

const uint8_t cube_texcoords[] = {
  0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 
  0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 
  0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 
  0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1
};

void make_cube(uint16_t size, uint8_t texture_index) 
{
  const uint16_t num_faces = 12;

  uint16_t uv_index = 0, vertex_index = 0;
  uint16_t hsize = size >> 1;

  for (uint16_t j = 0; j < num_faces; j++)
  for (uint16_t i = 0; i < 3; i++)
  {
    triangle_list[j].vertices[i].position.x = cube_vertices[vertex_index++] * hsize;
    triangle_list[j].vertices[i].position.y = cube_vertices[vertex_index++] * hsize;
    triangle_list[j].vertices[i].position.z = cube_vertices[vertex_index++] * hsize;
    triangle_list[j].vertices[i].u = cube_texcoords[uv_index++] << 7;
    triangle_list[j].vertices[i].v = cube_texcoords[uv_index++] << 6;
    triangle_list[j].texture_index = texture_index;
  }

  finalize_mesh(num_faces);
}

void effect_object_init()
{
  for (int i = 0; i < 256; i++)
  { 
    uint8_t r0 = i * i * i >> 16;
    uint8_t g0 = i * i * i >> 16;
    uint8_t b0 = i;
    uint8_t r1 = i;
    uint8_t g1 = i;
    uint8_t b1 = i;

    int t = 192;

    uint8_t r = lerp(r0, r1, t);
    uint8_t g = lerp(g0, g1, t);
    uint8_t b = lerp(b0, b1, t);

    r >>= 3;
    g >>= 3;
    b >>= 3;

    ((uint16_t *)0x5000000)[i] = (b << 10) | (g << 5) | r;   
  }

  new_mesh();
  make_cube(90, 0);

  current_model.flags = MODEL_BACKFACECULLING | MODEL_TEXTURED;
  current_model.position.z = -221;
}

void effect_object_destroy()
{
  
}


void effect_object_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  zoom_blur(target);

  current_model.euler_angles.x = sin(frame << 0) >> 1;
  current_model.euler_angles.z = cos(frame << 1) >> 1;

  render(&current_model);
}
 

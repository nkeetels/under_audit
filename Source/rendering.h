#ifndef __RENDERING_H
#define __RENDERING_H

#include "fixedmath.h"

#define VIEWPORT_WIDTH 240
#define VIEWPORT_HEIGHT 160
#define VIEWPORT_TOP 0
#define VIEWPORT_BOTTOM VIEWPORT_HEIGHT
#define VIEWPORT_LEFT 0
#define VIEWPORT_RIGHT VIEWPORT_WIDTH

#define MODEL_TEXTURED 1 << 1
#define MODEL_SORTFACES 1 << 2
#define MODEL_BACKFACECULLING 1 << 3
#define MODEL_FRONTFACECULLING 1 << 4
#define MODEL_ENVMAPPED 1 << 5

#define MAX_FACES 300
#define MAX_VERTICES 3 * MAX_FACES

typedef struct vertex_t {
  vector3_t position;
  int u, v;
} vertex_t;

typedef struct triangle_t {
  vertex_t vertices[3];
  uint16_t depth;
  uint8_t texture_index;
  struct triangle_t *next;
} triangle_t;

typedef struct model_t {
  struct triangle_t *triangles;
  vector3_t position;
  vector3_t euler_angles;
  int8_t flags;
} model_t;

vector3_t camera_position;
vector3_t euler_angles; /* y axis is up */

void clear_screen(uint8_t color);
void set_palette(const uint16_t *palette);
void set_camera_position(int16_t x, int16_t y, int16_t z);
void set_euler_angles(int16_t x, int16_t y, int16_t z);
void put_pixel(uint8_t x, uint8_t y, uint8_t color);
void line(int x0, int y0, int x1, int y1, uint8_t color);
void texture_triangle(triangle_t *face);
void flat_triangle(triangle_t *face);
void render(model_t *model);

extern triangle_t triangle_list[MAX_VERTICES]; 
extern model_t current_model;
extern uint16_t triangle_count;

#endif /* __RENDERING_H */
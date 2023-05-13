/*
  To do:
    - polygon clipping


*/

#include <gba_video.h>
#include "rendering.h"
#include "fixedmath.h"
#include "assets.h"

const uint16_t *texture_list[] = { tex_godwatbotBitmap, tex_subway1Bitmap, tex_subway2Bitmap,
                                   tex_subway3Bitmap, tex_subway4Bitmap, tex_subway5Bitmap, tex_subway6Bitmap,
                                   tex_litsphere2Bitmap, spr_logoBitmap };

extern uint16_t *framebuffer;
extern uint16_t *backbuffer;

EWRAM_DATA triangle_t triangle_list[MAX_VERTICES];
EWRAM_DATA triangle_t transformed_list[MAX_VERTICES]; 
EWRAM_DATA uint16_t draw_order[MAX_FACES];

model_t current_model;
uint16_t triangle_count = 0;

void (*rasterizer_func)(triangle_t *);

void set_palette(const uint16_t *palette)
{
  fast_copy(((unsigned short*)0x5000000), palette, 256);
}

void clear_screen(uint8_t color)
{
  fast_set(framebuffer, (color << 8)|(color), (VIEWPORT_WIDTH * VIEWPORT_HEIGHT) >> 1); 
}

void set_camera_position(int16_t x, int16_t y, int16_t z)
{
  camera_position.x = x;
  camera_position.y = y;
  camera_position.z = z;
}

void set_euler_angles(int16_t x, int16_t y, int16_t z)
{
  euler_angles.x = x;
  euler_angles.y = y;
  euler_angles.z = z; 
}

void init_renderer()
{
  set_camera_position(0, 0, 0);
  set_euler_angles(0, 0, 0);
}

void put_pixel(uint8_t x, uint8_t y, uint8_t color)
{

  if ((x < 0) | (x > 240) | (y < 0) | (y > 160))
    return;

  uint16_t *target = (uint16_t*)framebuffer + ((y * 240 + x) >> 1);

  if (x & 1) {
    *target = (*target & 0xFF) | (color << 8);
  }
  else {
    *target = (*target & ~0xFF) | color;
  }
}

void line(int x0, int y0, int x1, int y1, uint8_t color)
{
  if (y0 > y1) 
  {
    swap(x0, x1);
    swap(y0, y1);
  }

  int dx = x1 - x0;
  int dy = y1 - y0;

  int gradient = dx < 0 ? -1 : 1;
  dx *= gradient;

  put_pixel(x0, y0, color);

  if (dy == 0) 
  {
    while (dx--) put_pixel(x0 += gradient, y0, color);
    return;
  }

  if (dx == 0) 
  { 
    while (dy--) put_pixel(x0, y0++, color);
    return;
  }

  if (dx == dy) 
  {
    while (dy--) put_pixel(x0 += gradient, y0++, color);
    return;
  }
  
  uint16_t currentError;
  uint16_t error = 0;
  uint16_t intensity = 0;
   
  if (dy > dx) 
  {
    uint16_t fract = div(dx << 16, dy);

    while (--dy) 
    {
      currentError = error;   
      x0 += ((error += fract) <= currentError) ? gradient : 0;

      intensity = error >> 8;

      put_pixel(x0, ++y0, color + intensity);
      put_pixel(x0 + gradient, y0, color + (intensity ^ 255));
    }
  }
  else
  {
    uint16_t fract = div(dy << 16, dx);

    while (--dx) 
    {
      currentError = error; 
      y0 += ((error += fract) <= currentError) ? 1 : 0;

      intensity = error >> 8;

      put_pixel(x0 += gradient, y0, color + intensity);
      put_pixel(x0, y0 + 1, color + (intensity ^ 255));
    }
  }
  put_pixel(x1, y1, color);
}


void swap_vertex(vertex_t *a, vertex_t *b)
{
  vertex_t temp = *a;
  *a = *b;
  *b = temp;
}

void IWRAM_CODE horiz_line(int16_t x1, int16_t x2, int16_t y, uint16_t color)
{
  /* to do: smarter clipping */
  if (y < 0) return;
  if (y > 159) return;
  if (x1 < 0) x1 = 0;
  if (x2 > 238) x2 = 238;
  if (x2 < 0) return;
  if (x1 > 239) return;

  uint8_t *target = (uint8_t*)((uint32_t)framebuffer + x1 + y * VIEWPORT_WIDTH);

  int16_t length = x2 - x1 + 2;
  int16_t n = (length + 7) >> 3;

  switch (length & 7)
  {
    case 0: do {  *target++ = color;   
    case 7:       *target++ = color;
    case 6:       *target++ = color;
    case 5:       *target++ = color; 
    case 4:       *target++ = color;   
    case 3:       *target++ = color;
    case 2:       *target++ = color;
    case 1:       *target++ = color;  
    } while (--n > 0);
  }
}

void IWRAM_CODE flat_triangle(triangle_t *face) 
{
  int x1 = face->vertices[0].position.x;
  int y1 = face->vertices[0].position.y;
  int x2 = face->vertices[1].position.x;
  int y2 = face->vertices[1].position.y;  
  int x3 = face->vertices[2].position.x;
  int y3 = face->vertices[2].position.y;  

  if (y3 < y2)  
  {
    swap(x3, x2);
    swap(y3, y2);
  }
  if (y2 < y1) 
  {
    swap(x2, x1);
    swap(y2, y1);
  }
  if (y3 < y2) 
  { 
    swap(x3, x2);
    swap(y3, y2);
  }

  int d1 = y3 - y1;
  int d2 = y2 - y1;
  int d3 = y3 - y2;

  prevent_zero(d1);
  prevent_zero(d2);
  prevent_zero(d3);

  int deltaAC = div((x3 - x1) << 16, d1);
  int deltaAB = div((x2 - x1) << 16, d2);
  int deltaBC = div((x3 - x2) << 16, d3);
  
  int mid = (x1 << 16) + (y2 - y1) * deltaAC;

  int swapped = 0;
  if ((x2 << 16) < mid) 
  {
    swap(deltaAC, deltaAB);
    swapped = 1;
  }

  vector3_t n;
  normal(&n, &face->vertices[0].position, &face->vertices[1].position, &face->vertices[2].position);
  uint16_t color = div(abs(n.z), 5);

  int startx = (x1 << 16);
  int endx = startx;

  int y = 0;
  if (y1 != y2) 
  {
    for (y = y1; y < y2; y ++) 
    {
      horiz_line((startx+=deltaAC) >> 16, (endx+=deltaAB) >> 16, y, color);
    }
  }
  else if (y1 == y2) 
  {
    if (!swapped) 
    {
      startx = (x1 << 16);
      endx = (x2 << 16);
    } 
    else 
    {
      startx = (x2 << 16);
      endx = (x1 << 16);
    }
  }

  if (swapped) 
  {
    swap(deltaAB, deltaAC);
  }

  if ((x2 << 16) < mid) 
  {
    swap(deltaAC, deltaBC);
  }

  if (y2 != y3) 
  {
    for (y = y2; y <= y3; y++) 
    {
      horiz_line(startx >> 16, endx >> 16, y, color);
      startx += deltaAC;
      endx += deltaBC;
    }
  } 
  else {
    y = y2;
    horiz_line(startx >> 16, endx >> 16, y, color);
  }  
}

#define SWAPPED 1 << 1
void IWRAM_CODE texture_triangle(triangle_t *face)
{  
  if (face->vertices[1].position.y < face->vertices[0].position.y)
   swap_vertex(&face->vertices[0], &face->vertices[1]);
  if (face->vertices[2].position.y < face->vertices[0].position.y)
   swap_vertex(&face->vertices[0], &face->vertices[2]);
  if (face->vertices[2].position.y < face->vertices[1].position.y)
   swap_vertex(&face->vertices[1], &face->vertices[2]);

  if (face->vertices[0].position.y == face->vertices[2].position.y || 
      face->vertices[2].position.y < 0 || face->vertices[0].position.y >= 160) {
    return;
  }

  int x0 = face->vertices[0].position.x << 16;
  int y0 = face->vertices[0].position.y;  
  int u0 = face->vertices[0].u << 8;
  int v0 = face->vertices[0].v << 8;

  int x1 = face->vertices[1].position.x << 16;
  int y1 = face->vertices[1].position.y;
  int u1 = face->vertices[1].u << 8;
  int v1 = face->vertices[1].v << 8;

  int x2 = face->vertices[2].position.x << 16;
  int y2 = face->vertices[2].position.y;
  int u2 = face->vertices[2].u << 8;
  int v2 = face->vertices[2].v << 8;

  if (y0 == y1) {
    if (x1 < x0)  {
      swap(x0, x1);
      swap(u0, u1);
      swap(v0, v1);
    }

    y1++; 
  }

  if (y1 == y2) {
    y2++;
  }

  int length, dxAC, duAC, dvAC, dxBC, duBC, dvBC, dxAB, duAB, dvAB;

  length = y2 - y0;
  prevent_zero(length);

  dxAC = div((x2 - x0), length);
  duAC = div((u2 - u0), length);
  dvAC = div((v2 - v0), length);

  length = y1 - y0;
  prevent_zero(length);

  dxAB = div((x1 - x0), length);
  duAB = div((u1 - u0), length);
  dvAB = div((v1 - v0), length);

  length = y2 - y1;
  dxBC = div((x2 - x1), length);
  duBC = div((u2 - u1), length);
  dvBC = div((v2 - v1), length);

  uint8_t flags = 0;

  if (dxAB < dxAC) {
    flags |= SWAPPED;

    swap(dxAB, dxAC);
    swap(duAB, duAC);
    swap(dvAB, dvAC);
  }

  x2 = x1;
  u2 = u1;
  v2 = v1;
  x1 = x0;
  u1 = u0;
  v1 = v0;    

  uint16_t *texture = (uint16_t*)texture_list[face->texture_index];

  if (y0 < 0) {
    int overclip = y0;

    x0 -= overclip * dxAC;
    x1 -= overclip * dxAB;
    u0 -= overclip * duAC;
    u1 -= overclip * duAB;
    v0 -= overclip * dvAC;
    v1 -= overclip * dvAB;

    y0 = 0;
  }

  int dy, b0, b1, du, dv;

  dy = y1 - y0;
  b0 = x0 + (dy * dxAC);
  b1 = x1 + (dy * dxAB);
  length = ((b1 - b0) >> 16);

  prevent_zero(length);

  b0 = u0 + (dy * duAC);
  b1 = u1 + (dy * duAB);
  du = div(((b1 - b0) << 8), length) >> 7;

  b0 = v0 + (dy * dvAC);
  b1 = v1 + (dy * dvAB);
  dv = div(((b1 - b0) << 8), length) >> 7;
  
  int stride, n, y, tu, tv;
  uint16_t *line;

  if (y1 >= 0) {
    line = (uint16_t*)(framebuffer + 120 * y0);

    y = (y1 - y0);

    do {
      int clipped_x1 = min(118, x1 >> 17);
      int clipped_x0 = min(118, x0 >> 17);

      uint16_t* endx = (uint16_t*)(line + max(0, clipped_x1));
      uint16_t *curx = (uint16_t*)(line + max(0, clipped_x0));

      tu = u0;
      tv = v0;

      #define PLOT \
      { \
        *curx++ = texture[(((tu >> 9) ) + (((tv >> 9) ) << 8)) & 0x1FFF]; \
        tu += du; tv += dv; \
      }

      stride = endx - curx + 2;
      n = (stride + 7) >> 3;

      switch (stride & 7) {
        case 0: do { PLOT;   
        case 7: PLOT;
        case 6: PLOT;
        case 5: PLOT; 
        case 4: PLOT;   
        case 3: PLOT;
        case 2: PLOT;
        case 1: PLOT;  
        } while (--n); 
      }

      x0 += dxAC;
      x1 += dxAB;
      u0 += duAC;
      v0 += dvAC;

      if (curx >= (uint16_t*)(framebuffer + 120 * 159)) {
        return;
      }

      line += 120;
    } while (--y);
  }

  if (flags & SWAPPED) {
    x0 = x2;
    u0 = u2;
    v0 = v2;

    u1 += duAB * dy;
    v1 += dvAB * dy;

    dxAC = dxBC;
    duAC = duBC;
    dvAC = dvBC;

    dxBC = dxAB;
    duBC = duAB;
    dvBC = dvAB;
  }
  else {
    x1 = x2;
    u1 = u2;
    v1 = v2;
  }
        
  if (y1 < 0) {
    if (flags & SWAPPED) {
      int overclip = y1;

      x0 -= overclip * dxAC;
      x1 -= (y0 - 0) * dxBC;
      u0 -= overclip * duAC;
      v0 -= overclip * dvAC;
    }
    else {
      int overclip = y2;

      x0 -= overclip * dxAC;
      x1 -= (y1 - 0) * dxBC;
      u0 -= overclip * duAC;
      v0 -= overclip * dvAC;
    }

    y1 = 0;
  }

  line = (uint16_t*)(framebuffer + 120 * y1);
  y = (y2 - y1);

  do {
    int clipped_x1 = min(118, x1 >> 17);
    int clipped_x0 = min(118, x0 >> 17);
    uint16_t* endx = (uint16_t*)(line + max(0, clipped_x1));
    uint16_t *curx = (uint16_t*)(line + max(0, clipped_x0));

    tu = u0;
    tv = v0;

    stride = endx - curx + 2;
    n = (stride + 7) >> 3;
    
    switch (stride & 7) {
      case 0: do { PLOT;   
      case 7: PLOT;
      case 6: PLOT;
      case 5: PLOT; 
      case 4: PLOT;   
      case 3: PLOT;
      case 2: PLOT;
      case 1: PLOT;  
      } while (--n); 
    }
      
    x0 += dxAC;
    x1 += dxBC;
    u0 += duAC;
    v0 += dvAC;

    if (curx >= (uint16_t*)(framebuffer + 120 * 159)) {
      return;
    }

    line += 120;

  } while (--y);
}

void transform(model_t *model, vertex_t *v, matrix3x3_t m)
{    
  v->position.x -= camera_position.x;
  v->position.y -= camera_position.y;
  v->position.z -= camera_position.z;

  int nx = (m[0][0] * v->position.x + m[0][1] * v->position.y + m[0][2] * v->position.z) >> 8;
  int ny = (m[1][0] * v->position.x + m[1][1] * v->position.y + m[1][2] * v->position.z) >> 8;
  int nz = (m[2][0] * v->position.x + m[2][1] * v->position.y + m[2][2] * v->position.z) >> 8;

  v->position.x = nx;
  v->position.y = ny;
  v->position.z = nz;

  v->position.x += model->position.x;
  v->position.y += model->position.y;
  v->position.z += model->position.z;
}

void IWRAM_CODE project(vertex_t *v)
{
  v->position.x = div(v->position.x << 7, v->position.z - 64) + (VIEWPORT_WIDTH >> 1);
  v->position.y = div(v->position.y << 7, v->position.z - 64) + (VIEWPORT_HEIGHT >> 1);
}

void IWRAM_CODE swap_indices(uint16_t *a, uint16_t *b, uint16_t l, uint16_t r)
{
  uint16_t t = *a;
  *a = *b;
  *b = t;

  t = draw_order[l];
  draw_order[l] = draw_order[r];
  draw_order[r] = t;
}

void IWRAM_CODE quick_sort(uint16_t *list, uint16_t start, uint16_t end)
{
  uint16_t z, l, r;

  if (end > start + 1)
  {
    z = list[start];
    l = start + 1;
    r = end;

    while (l < r)
    {
      if (list[l] > z)
      { 
        l++;
      }
      else
      {
        r--;
        swap_indices(&list[l], &list[r], l, r);
      }
    }
    l--;
    swap_indices(&list[l], &list[start], l, start);
    quick_sort(list, start, l);
    quick_sort(list, r, end);
  }
} 

bool is_inside_viewport(triangle_t *t)
{
  return !((t->vertices[0].position.x < 0 && t->vertices[1].position.x < 0 && t->vertices[2].position.x < 0) ||
        (t->vertices[0].position.y < 0 && t->vertices[1].position.y < 0 && t->vertices[2].position.y < 0) ||
        (t->vertices[0].position.x >= VIEWPORT_RIGHT && t->vertices[1].position.x >= VIEWPORT_RIGHT && t->vertices[2].position.x >= VIEWPORT_RIGHT) || 
        (t->vertices[0].position.y >= VIEWPORT_HEIGHT && t->vertices[1].position.y >= VIEWPORT_HEIGHT && t->vertices[2].position.y >= VIEWPORT_HEIGHT)); 
}

bool is_clockwise(triangle_t *t) 
{
  return ((t->vertices[1].position.x - t->vertices[0].position.x) * 
          (t->vertices[2].position.y - t->vertices[1].position.y) - 
          (t->vertices[1].position.y - t->vertices[0].position.y) * 
          (t->vertices[2].position.x - t->vertices[1].position.x)) > 0;
}

bool is_behind_camera(triangle_t *t)
{
  return (t->vertices[0].position.z > 0 || t->vertices[1].position.z > 0 || t->vertices[2].position.z > 0);
}


int vector_length(int x, int y, int z)
{
  x >>= 8;
  y >>= 8;
  z >>= 8;

  return (sqrt(x * x + y * y + z * z) << 8);
}

void normalize_vector(int *x, int *y, int *z)
{
   int length = vector_length(*x, *y, *z);

   *x = div(*x, length);
   *y = div(*y, length);
   *z = div(*z, length);
}

extern uint32_t current_frame;

void IWRAM_CODE render(model_t *model)
{
  int i;
  int num_triangles = 0;
  matrix3x3_t m;

  //rotate(m, euler_angles.x, euler_angles.y, euler_angles.z);
  rotate(m, model->euler_angles.x, model->euler_angles.y, model->euler_angles.z);

  triangle_t *idx = model->triangles;
  while (idx != 0) {
    triangle_t face;

    for (i = 0; i < 3; i++) {
      face.vertices[i] = idx->vertices[i];

      transform(model, &face.vertices[i], m);

      face.texture_index = idx->texture_index;
    }

    if (!is_behind_camera(&face)) {
      if (is_clockwise(&face)) {
        for (i = 0; i < 3; i++) {
          project(&face.vertices[i]);
        }

        if (is_inside_viewport(&face)) {
#if 0
          if (model->flags & MODEL_ENVMAPPED) {
          for (i = 0; i < 3; i++) {
            int32_t x = face.vertices[i].position.x + camera_position.x - current_model.position.x;// - (VIEWPORT_WIDTH >> 1);
            int32_t y = face.vertices[i].position.y + camera_position.y - current_model.position.y;// - (VIEWPORT_HEIGHT >> 1);
            int32_t z = face.vertices[i].position.z + camera_position.z - current_model.position.z;

            normalize_vector(x, y, z);

            face.vertices[i].u = (64 + x) & 127;//((spherical((x + 64)))) & 127;
            face.vertices[i].v = (32 + y) & 63;//((spherical((y + 32)))) & 63;
          }
          }
#endif

          transformed_list[num_triangles++] = face;
        }
      }
    }
    idx = idx->next;
  }
  
  for (i = 0; i < num_triangles; i++)
    draw_order[i] = i;

  quick_sort(draw_order, 0, num_triangles);

  if (model->flags & MODEL_TEXTURED) {
    rasterizer_func = texture_triangle;
  }
  else {
    rasterizer_func = flat_triangle;
  }

  uint16_t triangle_index;
  for (i = 0; i < num_triangles; i++) {
    triangle_index = draw_order[i];
    rasterizer_func(&transformed_list[triangle_index]);
  }
}
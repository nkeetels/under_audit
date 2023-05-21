#include "effects.h"
#include "rendering.h"
#include "memory.h"
#include "fixedmath.h"
#include "sprites.h"
#include "assets.h"
#include "pimp_gba.h"


extern uint16_t *framebuffer;
extern uint16_t *backbuffer;

void effect_end_init()
{
  for (int i = 1; i < 255; ++i) {
    generated_palette[i] = (i >> 4 << 10) | (i >> 4 << 5) | i >> 4;
  }

  set_palette(generated_palette);
  
	clear_sprites();

  commit_sprites();
}

void effect_end_destroy()
{
  //rotate_background(0); 
  clear_buffers(0);
  pimp_gba_close();
  while (1) {}
}


void effect_end_update(uint16_t *target, uint32_t frame, uint16_t sync)
{
  rotate_background(-frame, 32 + (frame << 2));

  fast_copy(target, tex_logo_fullscreenBitmap, VIEWPORT_WIDTH * VIEWPORT_HEIGHT >> 1);

  add_palette((uint16_t*)0x5000000, generated_palette, frame % (rand() % 5));
}

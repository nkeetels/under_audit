#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include "assets.h"
#include "script.h"
#include "demo.h"
#include "rendering.h"
#include "vcount.h"
#include "pimp_gba.h"

#define CAMERA_MOVESPEED 15

int8_t prev_order = 0;
uint16_t sync_track;

#define FB (uint16_t*)0x6000000
#define BB (uint16_t*)0x600A000

uint16_t *framebuffer = BB;
uint16_t *backbuffer = FB;

static uint32_t current_frame = 0;

void vblank()
{
	pimp_gba_vblank();  
	pimp_gba_frame();
    
  ++current_frame;
}

void vcount()
{

}

void present()
{
  if (REG_DISPCNT & BACKBUFFER) {
    REG_DISPCNT &= ~BACKBUFFER;
    framebuffer = BB;
    backbuffer = FB;
  }
  else {
    REG_DISPCNT |= BACKBUFFER;
    framebuffer = FB;
    backbuffer = BB;
  } 
}

void update_camera()
{
  scanKeys();

  uint16_t key = keysDown();
/*
  if (key & KEY_LEFT) {
    euler_angles.z += CAMERA_MOVESPEED;
  }
  if (key & KEY_RIGHT) {
    euler_angles.z -= CAMERA_MOVESPEED;
  }
  if (key & KEY_UP) {
    camera_position.x -= (sin(euler_angles.z) * CAMERA_MOVESPEED) >> 8;
    camera_position.z -= (cos(euler_angles.z) * CAMERA_MOVESPEED) >> 8;
  }
  if (key & KEY_DOWN) {
    camera_position.x += (sin(euler_angles.z) * CAMERA_MOVESPEED) >> 8;
    camera_position.z += (cos(euler_angles.z) * CAMERA_MOVESPEED) >> 8;
  }
  if (key & KEY_L) {
    camera_position.y -= CAMERA_MOVESPEED*2;
  }
  if (key & KEY_R) {
    camera_position.y += CAMERA_MOVESPEED*2;
  }
  */

  if (key & KEY_LEFT) {
    euler_angles.x -= 8;
  }
  if (key & KEY_RIGHT) {
    euler_angles.x += 8;
  }
  if (key & KEY_UP) {
    camera_position.y -= CAMERA_MOVESPEED;
  }
  if (key & KEY_DOWN) {
    camera_position.y += CAMERA_MOVESPEED;
  }

}

void run_demo()
{
  pimp_gba_init((struct pimp_module*)underaudit_mod, (void*)underaudit_samples);

  irqInit();
  irqEnable(IRQ_VBLANK | IRQ_HBLANK);
  irqSet(IRQ_VBLANK, vblank);
  irqSet(IRQ_HBLANK, vcount);

  SetMode(MODE_4 | BG2_ON | OBJ_ENABLE | OBJ_1D_MAP | BG_MOSAIC);

  start_script();

  while (1) {
    VBlankIntrWait();

//    update_camera();
    
    uint8_t row = pimp_gba_get_row();
    uint8_t order = pimp_gba_get_order();
    if ((order != prev_order) && (sync_track == 0)) {
      next_effect();
      prev_order = order;
    }

    tick_script(row);
    present();
  }
}
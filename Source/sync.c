#include "sprites.h"
#include "sync.h"

int8_t sync_timer = -1;

void sync_none(uint32_t frame, uint16_t param)
{
}

void sync_camera(uint32_t frame, uint16_t param)
{ 
}

void sync_glitch(uint32_t frame, uint16_t param)
{
}

void sync_mosiac(uint32_t frame, uint16_t param)
{
  uint8_t trigger = 0;
  if ((param & 15) == 0) {
    sync_timer = 16;
  }

  if (sync_timer >= 0) {
    int16_t scale = 16 - (sync_timer--);
    set_mosaic(scale, scale, scale, scale);
  }
  else {
    set_mosaic(0, 0, 0, 0);
  }
}

void sync_blur(uint32_t frame, uint16_t param)
{
}

void sync_custom(uint32_t frame, uint16_t param)
{

}

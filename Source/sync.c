#include "sprites.h"
#include "sync.h"

int8_t sync_timer = -1;

const uint8_t kick_pattern[10] = {0x00, 0x08, 0x0E, 0x12, 0x18, 0x20, 0x28, 0x2E, 0x32, 0x38};
const uint8_t snare_pattern[4] = {0x0C, 0x1C, 0x2C, 0x3C};

int sync_trigger(const uint8_t *pattern, const uint8_t pattern_size, const uint8_t value)
{
  for (int i = 0; i < pattern_size; ++i) {
    if (pattern[i] == value) {
      return 1;
    }
  }
  return 0;
}


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

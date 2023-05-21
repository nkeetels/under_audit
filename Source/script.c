#include "demo.h"
#include "script.h"
#include "effects.h"

// DENISE LINOLD
#define NUM_EFFECTS 13

uint16_t current_frame = 0;
uint8_t current_effect = 0;

extern uint16_t *framebuffer;

const static struct scene_t scene_list[] = {  
  effect_underaudit,
  effect_spacewous, 
  effect_spacewous, 
  effect_subway,  
  effect_spheredistort,
  effect_spheredistort,
  effect_ball,
  effect_kaleido,
  effect_greetings,
  effect_greetings,
  //effect_sky,
  effect_tunnel,
  effect_planes,
  effect_end,

};


void reset_script()
{
  current_frame = 0;
  current_effect = 0;
}

void start_script()
{
  reset_script();

  scene_list[current_effect].init();
}

void advance_script()
{
  ++current_effect;
  current_effect %= NUM_EFFECTS;
}

void tick_script(uint8_t pulse)
{ 
  scene_list[current_effect].update(framebuffer, current_frame, pulse);

  ++current_frame;
}

void next_effect()
{
  scene_list[current_effect].destroy();

  advance_script();

  scene_list[current_effect].init();

}
#ifndef __EFFECT_H
#define __EFFECT_H

#include <stdint.h>
#include "demo.h"

void move_table(uint16_t *target, uint8_t *texture, uint16_t *table, int16_t xoffset, int16_t yoffset, uint16_t t);
void uv_table(uint16_t *target, uint8_t *texture, int *table, uint16_t t);

void bloom(uint16_t *target, const uint8_t bloom_bias, const uint8_t bloom_threshold);
void motion_blur(uint16_t *target);
void zoom_blur(uint16_t *target);

void fade_palette(uint16_t *dest, const uint16_t *palette, uint8_t red, uint8_t green, uint8_t blue, uint8_t v);
void invert_palette(uint16_t *dest, const uint16_t *palette, uint8_t v);
void scale_palette(uint16_t *dest, const uint16_t *palette, uint8_t v);
void add_palette(uint16_t *dest, const uint16_t *palette, uint8_t v);

void rotate_background(uint16_t angle, uint16_t zoom);

void effect_ball_init();
void effect_ball_destroy();
void effect_ball_update(uint16_t *target, uint32_t frame, uint16_t sync);

void effect_spacewous_init();
void effect_spacewous_destroy();
void effect_spacewous_update(uint16_t *target, uint32_t frame, uint16_t sync);

void effect_kaleido_init();
void effect_kaleido_destroy();
void effect_kaleido_update(uint16_t *target, uint32_t frame, uint16_t sync);

void effect_spheredistort_init();
void effect_spheredistort_destroy();
void effect_spheredistort_update(uint16_t *target, uint32_t frame, uint16_t sync);

void effect_planes_init();
void effect_planes_destroy();
void effect_planes_update(uint16_t *target, uint32_t frame, uint16_t sync);

void effect_tunnel_init();
void effect_tunnel_destroy();
void effect_tunnel_update(uint16_t *target, uint32_t frame, uint16_t sync);

void effect_sky_init();
void effect_sky_destroy();
void effect_sky_update(uint16_t *target, uint32_t frame, uint16_t sync);

void effect_shinything_init();
void effect_shinything_destroy();
void effect_shinything_update(uint16_t *target, uint32_t frame, uint16_t sync);

void effect_underaudit_init();
void effect_underaudit_destroy();
void effect_underaudit_update(uint16_t *target, uint32_t frame, uint16_t sync);

void effect_subway_init();
void effect_subway_destroy();
void effect_subway_update(uint16_t *target, uint32_t frame, uint16_t sync);

void effect_greetings_init();
void effect_greetings_destroy();
void effect_greetings_update(uint16_t *target, uint32_t frame, uint16_t sync);

void effect_end_init();
void effect_end_destroy();
void effect_end_update(uint16_t *target, uint32_t frame, uint16_t sync);

static const struct scene_t effect_ball = {
	effect_ball_init,
	effect_ball_update,
	effect_ball_destroy,
};

static const struct scene_t effect_spacewous = {
	effect_spacewous_init,
	effect_spacewous_update,
	effect_spacewous_destroy,
};

static const struct scene_t effect_kaleido = {
	effect_kaleido_init,
	effect_kaleido_update,
	effect_kaleido_destroy,
};

static const struct scene_t effect_spheredistort = {
	effect_spheredistort_init,
	effect_spheredistort_update,
	effect_spheredistort_destroy,
};

static const struct scene_t effect_planes = {
	effect_planes_init,
	effect_planes_update,
	effect_planes_destroy,
};

static const struct scene_t effect_tunnel = {
	effect_tunnel_init,
	effect_tunnel_update,
	effect_tunnel_destroy,
};

static const struct scene_t effect_sky = {
	effect_sky_init,
	effect_sky_update,
	effect_sky_destroy,
};

static const struct scene_t effect_shinything = {
	effect_shinything_init,
	effect_shinything_update,
	effect_shinything_destroy,
};

static const struct scene_t effect_underaudit = {
	effect_underaudit_init,
	effect_underaudit_update,
	effect_underaudit_destroy,
};

static const struct scene_t effect_subway = {
	effect_subway_init,
	effect_subway_update,
	effect_subway_destroy,
};

static const struct scene_t effect_greetings = {
	effect_greetings_init,
	effect_greetings_update,
	effect_greetings_destroy,
};

static const struct scene_t effect_end = {
	effect_end_init,
	effect_end_update,
	effect_end_destroy,
};


extern int uvtable[10 * 8 * 3];
extern unsigned short generated_palette[256];


#endif /* __EFFECT_H */
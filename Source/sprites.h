#ifndef __SPRITES_H
#define __SPRITES_H

#include <stdint.h>

void commit_sprites();
void clear_sprites();
void set_sprites_transparency(int value);
void set_sprite_palette(const uint16_t *palette);
void set_sprite(uint8_t id, int xpos, int ypos, uint8_t size, uint8_t rotate, uint8_t transparent, const uint16_t *image);
void rotate_sprite(uint8_t id, uint8_t angle, int xscale, int yscale);
void set_overlay(const uint16_t *image1, const uint16_t *image2, const uint16_t *image3, const uint16_t *image4, const uint16_t *palette);
void set_sprite_position(const uint8_t id, int xpos, int ypos);
void set_mosaic(const uint16_t bgx, const uint16_t bgy, const uint16_t objx, const uint16_t objy);

#endif /* __SPRITES_H */

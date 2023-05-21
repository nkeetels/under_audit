#ifndef __SYNC_H
#define __SYNC_H

#include <stdint.h>

extern const uint8_t kick_pattern[10];
extern const uint8_t snare_pattern[4];

int sync_trigger(const uint8_t *pattern, const uint8_t pattern_size, const uint8_t value);

void sync_none(uint32_t frame, uint16_t param);
void sync_camera(uint32_t frame, uint16_t param);
void sync_glitch(uint32_t frame, uint16_t param);
void sync_mosiac(uint32_t frame, uint16_t param);
void sync_blur(uint32_t frame, uint16_t param);
void sync_custom(uint32_t frame, uint16_t param);

#endif /* __SYNC_H */

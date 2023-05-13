#ifndef __SYNC_H
#define __SYNC_H

#include <stdint.h>

void sync_none(uint32_t frame, uint16_t param);
void sync_camera(uint32_t frame, uint16_t param);
void sync_glitch(uint32_t frame, uint16_t param);
void sync_mosiac(uint32_t frame, uint16_t param);
void sync_blur(uint32_t frame, uint16_t param);
void sync_custom(uint32_t frame, uint16_t param);

#endif /* __SYNC_H */

#ifndef __MESHGENERATOR_H
#define __MESHGENERATOR_H

#include <stdint.h>

#define MESH_STATIC 1 << 1
#define MESH_VERTEXDISPLACE 1 << 2

void new_mesh();
void finalize_mesh(const int numFaces);
void subdivide_icosahedron(uint16_t offset, uint8_t mode, uint8_t texture_index);

#endif /* __MESHGENERATOR_H */
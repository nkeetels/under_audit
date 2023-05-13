#include "meshgenerator.h"
#include "rendering.h"
#include "fixedmath.h"

void (*displacement_func)(vertex_t *, uint16_t);

const int16_t X = 34;
const int16_t Z = 64;
const int16_t N = 0;

const vector3_t ico_verts[] = {
  {-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z}, {N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X}, {Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
};

const uint8_t ico_indices[] = {
  0,4,1,0,9,4,9,5,4,4,5,8,4,8,1,8,10,1,8,3,10,5,3,8,5,2,3,2,7,3,7,10,3,7,6,10,7,11,6,11,0,6,0,1,6,6,1,10,9,0,11,9,11,2,9,2,5,7,2,11
};

void finalize_mesh(const int numFaces)
{
  current_model.triangles = &triangle_list[0];
  for (int i = 0; i < numFaces - 1; i++) {
    triangle_list[i].next = &triangle_list[i+1];
  }
    
  current_model.triangles[numFaces - 1].next = 0;
}

void new_mesh()
{
  triangle_count = 0;
  current_model.triangles[0].next = 0;
}

void displace_vertex(vertex_t *in, uint16_t value)
{
  int x = in->position.x;
  int y = in->position.y;
  int z = in->position.z;

  int rep = 2;
	int scale = sin(((x * rep) + (value >> 0))) + 
              sin(((y * rep) + (value << 1))) + 
              sin(((z * rep) + (value >> 1)));

  in->position.x += (in->position.x) * (scale >> 4) >> 5; 
  in->position.y += (in->position.y) * (scale >> 4) >> 5; 
  in->position.z += (in->position.z) * (scale >> 4) >> 5; 
}

void displace_none(vertex_t *in, uint16_t value)
{
}

void subdivide_icosahedron(uint16_t offset, uint8_t mode, uint8_t texture_index)
{
  int i = 0, j = 0;
  vector3_t edge1, edge2, edge3;
  int16_t index = 0;

  switch (mode) {
    case MESH_VERTEXDISPLACE:
      displacement_func = displace_vertex;
      break;
    case MESH_STATIC:
      displacement_func = displace_none;
    default:
    break;
  }

  for (i = 0; i < 20; i++) {
    int16_t t0x0 = ico_verts[ico_indices[index + 0]].x;
    int16_t t0y0 = ico_verts[ico_indices[index + 0]].y;
    int16_t t0z0 = ico_verts[ico_indices[index + 0]].z;

    int16_t t0x1 = ico_verts[ico_indices[index + 1]].x;
    int16_t t0y1 = ico_verts[ico_indices[index + 1]].y;
    int16_t t0z1 = ico_verts[ico_indices[index + 1]].z;

    int16_t t0x2 = ico_verts[ico_indices[index + 2]].x;
    int16_t t0y2 = ico_verts[ico_indices[index + 2]].y;
    int16_t t0z2 = ico_verts[ico_indices[index + 2]].z;
  
    edge1.x = t0x0 + ((t0x1 - t0x0) >> 1);
    edge1.y = t0y0 + ((t0y1 - t0y0) >> 1);
    edge1.z = t0z0 + ((t0z1 - t0z0) >> 1);

    edge2.x = t0x1 + ((t0x2 - t0x1) >> 1);
    edge2.y = t0y1 + ((t0y2 - t0y1) >> 1);
    edge2.z = t0z1 + ((t0z2 - t0z1) >> 1);

    edge3.x = t0x2 + ((t0x0 - t0x2) >> 1);
    edge3.y = t0y2 + ((t0y0 - t0y2) >> 1);
    edge3.z = t0z2 + ((t0z0 - t0z2) >> 1);

    triangle_list[j].texture_index = texture_index;
    triangle_list[j].vertices[0].position.x = edge3.x;
    triangle_list[j].vertices[0].position.y = edge3.y;
    triangle_list[j].vertices[0].position.z = edge3.z;
    triangle_list[j].vertices[1].position.x = t0x0;
    triangle_list[j].vertices[1].position.y = t0y0;
    triangle_list[j].vertices[1].position.z = t0z0;
    triangle_list[j].vertices[2].position.x = edge1.x;
    triangle_list[j].vertices[2].position.y = edge1.y;
    triangle_list[j].vertices[2].position.z = edge1.z;
    displacement_func(&triangle_list[j].vertices[0], offset);
    displacement_func(&triangle_list[j].vertices[1], offset);
    displacement_func(&triangle_list[j].vertices[2], offset);

    j++;

    triangle_list[j].texture_index = texture_index;
    triangle_list[j].vertices[0].position.x = edge1.x;
    triangle_list[j].vertices[0].position.y = edge1.y;
    triangle_list[j].vertices[0].position.z = edge1.z;
    triangle_list[j].vertices[1].position.x = t0x1;
    triangle_list[j].vertices[1].position.y = t0y1;
    triangle_list[j].vertices[1].position.z = t0z1;
    triangle_list[j].vertices[2].position.x = edge2.x;
    triangle_list[j].vertices[2].position.y = edge2.y;
    triangle_list[j].vertices[2].position.z = edge2.z;
    displacement_func(&triangle_list[j].vertices[0], offset);
    displacement_func(&triangle_list[j].vertices[1], offset);
    displacement_func(&triangle_list[j].vertices[2], offset);

    j++;

    triangle_list[j].texture_index = texture_index;
    triangle_list[j].vertices[0].position.x = edge2.x;
    triangle_list[j].vertices[0].position.y = edge2.y;
    triangle_list[j].vertices[0].position.z = edge2.z;
    triangle_list[j].vertices[1].position.x = t0x2;
    triangle_list[j].vertices[1].position.y = t0y2;
    triangle_list[j].vertices[1].position.z = t0z2;
    triangle_list[j].vertices[2].position.x = edge3.x;
    triangle_list[j].vertices[2].position.y = edge3.y;
    triangle_list[j].vertices[2].position.z = edge3.z;
    displacement_func(&triangle_list[j].vertices[0], offset);
    displacement_func(&triangle_list[j].vertices[1], offset);
    displacement_func(&triangle_list[j].vertices[2], offset);

    j++;

    triangle_list[j].texture_index = texture_index;
    triangle_list[j].vertices[0].position.x = edge1.x;
    triangle_list[j].vertices[0].position.y = edge1.y;
    triangle_list[j].vertices[0].position.z = edge1.z;
    triangle_list[j].vertices[1].position.x = edge2.x;
    triangle_list[j].vertices[1].position.y = edge2.y;
    triangle_list[j].vertices[1].position.z = edge2.z;
    triangle_list[j].vertices[2].position.x = edge3.x;
    triangle_list[j].vertices[2].position.y = edge3.y;
    triangle_list[j].vertices[2].position.z = edge3.z;
    displacement_func(&triangle_list[j].vertices[0], offset);
    displacement_func(&triangle_list[j].vertices[1], offset);
    displacement_func(&triangle_list[j].vertices[2], offset);

    j++;
    
    index += 3;
  }

  finalize_mesh(79+1);
}

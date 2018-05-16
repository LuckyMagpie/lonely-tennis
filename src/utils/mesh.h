#ifndef MESH_H
#define MESH_H

#include <stdbool.h>

#include <cglm/cglm.h>

#include "vector.h"

bool load_obj(const char* path, dumb_opengl_vector_t** vertices, dumb_opengl_vector_t** uvs, dumb_opengl_vector_t** normals);
void mesh_find_maxmin(dumb_opengl_vector_t* vertices, vec3 max, vec3 min);

#endif /* ifndef MESH_H */

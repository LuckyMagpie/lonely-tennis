#ifndef MESH_H
#define MESH_H

#include <stdbool.h>

#include <cglm/cglm.h>

#include "vector.h"

bool load_obj(const char* path, dumb_opengl_vector_t** vertices, dumb_opengl_vector_t** uvs, dumb_opengl_vector_t** normals);

#endif /* ifndef MESH_H */

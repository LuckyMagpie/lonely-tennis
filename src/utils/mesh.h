#ifndef MESH_H
#define MESH_H

#include <stdbool.h>

#include <cglm/cglm.h>

#include "vector.h"

bool load_obj(const char* path, vector_t* uv, vector_t* normals, vector_t* vertices);

#endif /* ifndef MESH_H */

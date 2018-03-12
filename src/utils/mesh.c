#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <cglm/cglm.h>

#include "vector.h"
#include "mesh.h"

bool load_obj(const char* path, vector_t* uvs, vector_t* normals, vector_t* vertices)
{
   FILE* file = fopen(path, "r");

    if (!file) {
       fprintf(stderr, "File not found") ;
       return false;
    }

    char flag[3];
    vector_t* tmp_uvs = vector_init();
    vector_t* tmp_normals = vector_init();
    vector_t* tmp_vertices = vector_init();
    vector_t* uv_indices = vector_init();
    vector_t* normal_indices = vector_init();
    vector_t* vertex_indices = vector_init();

    while(fscanf(file, "%2s", flag) != EOF) {
        if (strcmp(flag, "v") == 0)  {
            float* vertex = malloc(sizeof(float) * 3);
            fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
            vector_push_back(tmp_vertices, vertex);
        } else if (strcmp(flag, "vt") == 0) {
            float* uv = malloc(sizeof(float) * 2);
            fscanf(file, "%f %f\n", &uv[0], &uv[1]);
            vector_push_back(tmp_uvs, uv);
        } else if (strcmp(flag, "vn") == 0) {
            float* normal  = malloc(sizeof(float) * 3);
            fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
            vector_push_back(tmp_normals, normal);
        } else if (strcmp(flag, "f") == 0) {
            unsigned int vertex_index[3];
            unsigned int uv_index[3];
            unsigned int normal_index[3];
            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex_index[0], &uv_index[0], &normal_index[0],
                                                         &vertex_index[1], &uv_index[1], &normal_index[1],
                                                         &vertex_index[2], &uv_index[2], &normal_index[2]);

            for (int i = 0; i < 3; i++) {
                unsigned int* vertex_i = malloc(sizeof(unsigned int));
                unsigned int* uv_i = malloc(sizeof(unsigned int));
                unsigned int* normal_i = malloc(sizeof(unsigned int));

                *vertex_i = vertex_index[i];
                *uv_i = uv_index[i];
                *normal_i = normal_index[i];

                vector_push_back(vertex_indices, vertex_i);
                vector_push_back(uv_indices, uv_i);
                vector_push_back(normal_indices, normal_i);
            }
        } else {
           fscanf(file, "%*[^\n]\n", NULL);
        }
    }

    fclose(file);

    for (unsigned int i = 0; i < vertex_indices->size; i++) {
        unsigned int vertex_index = *(unsigned int*)vector_get(vertex_indices, i);
        unsigned int uv_index = *(unsigned int*)vector_get(uv_indices, i);
        unsigned int normal_index = *(unsigned int*)vector_get(normal_indices, i);

        float* vertex = vector_get(tmp_vertices, vertex_index-1);
        float* uv = vector_get(tmp_uvs, uv_index-1);
        float* normal = vector_get(tmp_normals, normal_index-1);

        float* copy_vertex = malloc(sizeof(float) * 3);
        float* copy_uv = malloc(sizeof(float) * 2);
        float* copy_normal = malloc(sizeof(float) * 3);

        vector_push_back(vertices, memcpy(copy_vertex, vertex, sizeof(float) * 3));
        vector_push_back(uvs, memcpy(copy_uv, uv, sizeof(float) * 2));
        vector_push_back(normals, memcpy(copy_normal, normal, sizeof(float) * 3));
    }

    vector_foreach(tmp_uvs, &vector_generic_item_free);
    vector_free(tmp_uvs);

    vector_foreach(tmp_normals, &vector_generic_item_free);
    vector_free(tmp_normals);

    vector_foreach(tmp_vertices, &vector_generic_item_free);
    vector_free(tmp_vertices);

    vector_foreach(uv_indices, &vector_generic_item_free);
    vector_free(uv_indices);

    vector_foreach(normal_indices, &vector_generic_item_free);
    vector_free(normal_indices);

    vector_foreach(vertex_indices, &vector_generic_item_free);
    vector_free(vertex_indices);

    vector_trim(vertices);
    vector_trim(uvs);
    vector_trim(normals);

    return true;
}

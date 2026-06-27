#ifndef VISUALX_MESH_H

#define VISUALX_MESH_H

#include <containers/core_vector.h>

#include <stddef.h>

typedef struct VISUALX_MESH_STRUCT
{
    vector_t vertices;
    vector_t indices;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
} mesh_t;

#endif
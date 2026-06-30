#ifndef VISUALX_MESH_H

#define VISUALX_MESH_H

#include <core.h>

typedef object_t mesh_t;

extern class_t Mesh_Class;

void Mesh_Class_Init();
void Mesh_Class_Destroy();

/*typedef struct VISUALX_MESH_STRUCT
{
    vector_t vertices;
    vector_t indices;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
} mesh_t;*/

#endif
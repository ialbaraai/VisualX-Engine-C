#ifndef VISUALX_SCENE_H

#define VISUALX_SCENE_H

#include <core.h>

#include "shader.h"

typedef struct VISUALX_SCENE_STRUCT
{
    string_t name;

    object_t shader;
    
    vector_t scene_objects;
    vector_t scene_lights;
} scene_t;

#endif
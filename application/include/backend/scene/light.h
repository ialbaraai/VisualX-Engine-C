#ifndef VISUALX_LIGHT_H

#define VISUALX_LIGHT_H

#include <core/core_string.h>
#include <cglm.h>

typedef struct VISUALX_LIGHT_STRUCT
{
    string_t name;

    vec3 position;
    vec3 direction;
    
    vec3 color;
    
    float intensity;
} light_t;

#endif
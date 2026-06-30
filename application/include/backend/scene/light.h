#ifndef VISUALX_LIGHT_H

#define VISUALX_LIGHT_H

#include <core.h>
#include <cglm.h>

typedef object_t light_t;

extern class_t Light_Class;

void Light_Class_Init();
void Light_Class_Destroy();

/*typedef struct VISUALX_LIGHT_STRUCT
{
    string_t name;

    vec3 position;
    vec3 direction;
    
    vec3 color;
    
    float intensity;
} light_t;*/

#endif
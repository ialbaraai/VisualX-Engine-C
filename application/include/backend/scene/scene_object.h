#ifndef VISUALX_SCENE_OBJECT_H

#define VISUALX_SCENE_OBJECT_H

#include <core/core_string.h>
#include <containers/core_vector.h>
#include <oop/core_oop.h>

#include <cglm.h>

#include "transform.h"
#include "../scripting/script.h"
#include "../rendering/mesh.h"

typedef struct VISUALX_OBJECT_H
{
    string_t name;

    transform_t transform;
    mesh_t mesh;

    object_t shader;
    vector_t scripts;
} scene_object_t;

#endif
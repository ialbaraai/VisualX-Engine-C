#ifndef VISUALX_LAUNCHER_H

#define VISUALX_LAUNCHER_H

#include <core.h>
#include "../backend/shader.h"

typedef struct VISUALX_GAME_STRUCT
{
    string_t name;
    string_t project_path;

    string_t vertex_shader_path;
    string_t fragment_shader_path;

    int width;
    int height;
} launcher_t;

#endif
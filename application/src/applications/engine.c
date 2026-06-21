#include "../../include/applications/engine.h"

#include <stdio.h>
#include <stdbool.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

void engine_init(engine_t* engine)
{
    if (!engine) return;

    core_string_init_data(&engine->name, 32, "VisualX Engine");
}

int engine_run(engine_t* engine)
{
    return 0;
}

void engine_destroy(engine_t* engine)
{
    if (engine)
    {
        core_string_destroy(&engine->name);
    }
}
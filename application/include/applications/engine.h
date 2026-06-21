#ifndef VISUALX_ENGINE_H

#define VISUALX_ENGINE_H

#include <core.h>

typedef struct VISUALX_ENGINE_STRUCT
{
    string_t name;
} engine_t;

void engine_init(engine_t* engine);
int engine_run(engine_t* engine);
void engine_destroy(engine_t* engine);

#endif
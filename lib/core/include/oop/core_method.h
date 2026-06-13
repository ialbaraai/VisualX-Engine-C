#ifndef CORE_METHOD_H

#define CORE_METHOD_H

#include "core_oop_types.h"
#include "core_oop_visibility.h"

#include "../core_backend.h"
#include "../core/core_string.h"

typedef void(*method_function_t)(object_t* this, int argc, void** argv);

typedef struct CORE_METHOD_STRUCT
{
    string_t name;
    visibility_t visibility;

    method_function_t function;
} method_t;

int core_method_init(method_t* method, const char* name, const method_function_t function, const visibility_t visibility);

void core_method_destroy(method_t* method);

void core_method_copy_callback(void* destination, const void* source);
void core_method_destroy_callback(void* object);

#endif
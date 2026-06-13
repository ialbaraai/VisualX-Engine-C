#include "../../include/oop/core_method.h"

#include <string.h>

int core_method_init(method_t* method, const char* name, const method_function_t function, const visibility_t visibility)
{
    if (!method) return 0;
    if (method->function) return 0;
    if (!name) return 0;
    
    if (method->name.data)
    {
        core_string_set(&method->name, name);
    }
    else
    {
        core_string_init_data(&method->name, strlen(name) + 1, name);
    }

    method->function = function;
    method->visibility = visibility;

    return 1;
}

void core_method_destroy(method_t* method)
{
    if (!method) return;

    if (method->name.data)
    {
        core_string_destroy(&method->name);
    }

    method->function = NULL;
    method->visibility = 0;
}

void core_method_copy_callback(void* destination, const void* source)
{
    if (destination && source)
    {
        const method_t* src = (method_t*)source;

        method_t copy = {0};
        if (core_method_init(&copy, src->name.data, src->function, src->visibility))
        {
            memcpy(destination, &copy, sizeof(method_t));
        }
    }
}
void core_method_destroy_callback(void* object)
{
    if (object) core_method_destroy((method_t*)object);
}
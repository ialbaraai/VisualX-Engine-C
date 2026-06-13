#ifndef CORE_MEMBER_H

#define CORE_MEMBER_H

#include "core_oop_types.h"
#include "core_oop_visibility.h"

#include "../core_backend.h"
#include "../core/core_string.h"

typedef struct CORE_MEMBER_STRUCT
{
    void* value;
    size_t size;

    string_t name;
    visibility_t visibility;

    void (*copy_function)(void* destination, const void* source);
    void (*destroy_function)(void* object);
} member_t;

int core_member_init(member_t* member, const char* name, const size_t value_size, const visibility_t visibility, copy_function_t copy_function, destroy_function_t destroy_function);
int core_member_init_data(member_t* member, const char* name, const size_t value_size, const visibility_t visibility, copy_function_t copy_function, destroy_function_t destroy_function, const void* value);

int core_member_set(member_t* member, const void* new_value);

void core_member_destroy(member_t* member);

void core_member_copy_callback(void* destination, const void* source);
void core_member_destroy_callback(void* object);

#endif
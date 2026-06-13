#ifndef CORE_OBJECT_H

#define CORE_OBJECT_H

#include "core_oop_types.h"

#include "../core/core_string.h"

typedef struct CORE_OBJECT_STRUCT
{
    class_t* class;
    string_t name;

    member_t* members;
    size_t members_size;

    method_t* methods;
    size_t methods_size;
} object_t;

int core_object_init(object_t* object, const class_t* base_class, const char* name, const size_t members_size, const size_t methods_size, const member_t* members, const method_t* methods);

int core_object_call(object_t* object, const char* method_name, int argc, void** argv);

int core_object_member_at(object_t* object, const char* member_name, member_t* target);
member_t* core_object_member_get(object_t* object, const char* member_name);

int core_object_method_at(object_t* object, const char* method_name, method_t* target);
method_t* core_object_method_get(object_t* object, const char* method_name);

void core_object_destroy(object_t* object);

void core_object_copy_callback(void* destination, const void* source);
void core_object_destroy_callback(void* object);

#endif
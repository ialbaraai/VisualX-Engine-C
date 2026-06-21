#ifndef CORE_CLASS_H

#define CORE_CLASS_H

#include "core_oop_types.h"

#include "../core/core_string.h"

typedef struct CORE_CLASS_STRUCT
{
    class_t* base_class;
    string_t name;

    member_t* static_members;
    size_t static_members_size;

    method_t* static_methods;
    size_t static_methods_size;

    int (*constructor)(object_t* instance, int argc, void** argv);
    void (*destructor)(object_t* instace);
} class_t;

int core_class_init(class_t* class, class_t* base_class, const char* name, const size_t static_members_size, const size_t static_methods_size, const member_t* static_members, const method_t* static_methods, int(*constructor)(object_t* object, int argc, void** argv), void(*destructor)(object_t* object));

int core_class_call(class_t* class, const char* static_method_name, int argc, void** argv);

int core_class_static_member_at(class_t* class, const char* static_member_name, member_t* target);
member_t* core_class_static_member_get(class_t* class, const char* static_member_name);

int core_class_static_method_at(class_t* class, const char* static_method_name, method_t* target);
method_t* core_class_static_method_get(class_t* class, const char* static_method_name);

void core_class_destroy(class_t* class);

void core_class_copy_callback(void* destination, const void* source);
void core_class_destroy_callback(void* object);

#endif
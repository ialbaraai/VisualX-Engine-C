#include "../../include/oop/core_object.h"

#include "../../include/oop/core_member.h"
#include "../../include/oop/core_method.h"

#include <stdlib.h>
#include <string.h>

int core_object_init(object_t* object, class_t* base_class, const char* name, const size_t members_size, const size_t methods_size, const member_t* members, const method_t* methods)
{
    if (!object) return 0;
    if (object->members || object->methods) return 0;
    if (!name) return 0;

    if (object->name.data)
    {
        core_string_set(&object->name, name);
    }
    else
    {
        core_string_init_data(&object->name, strlen(name) + 1, name);
    }

    object->class = base_class;

    object->members_size = members_size;
    object->members = malloc(members_size * sizeof(member_t));

    object->methods_size = methods_size;
    object->methods = malloc(methods_size * sizeof(method_t));

    if (!object->members)
    {
        object->members_size = 0;

        return 0;
    }

    if (!object->methods)
    {
        object->methods_size = 0;

        return 0;
    }

    for (size_t i = 0; i < object->members_size; ++i)
    {
        core_member_copy_callback(&object->members[i], &members[i]);
    }
    for (size_t i = 0; i < object->methods_size; ++i)
    {
        core_method_copy_callback(&object->methods[i], &methods[i]);
    }

    return 1;
}

int core_object_call(object_t* object, const char* method_name, int argc, void** argv)
{
    if (!object) return 0;
    if (!object->methods) return 0;
    if (!method_name) return 0;

    for (size_t i = 0; i < object->methods_size; ++i)
    {
        if (strcmp(method_name, object->methods[i].name.data) == 0)
        {
            object->methods[i].function(object, argc, argv);

            return 1;
        }
    }

    return 0;
}

int core_object_member_at(object_t* object, const char* member_name, member_t* target)
{
    if (!object) return 0;
    if (!object->members) return 0;
    if (!member_name) return 0;
    
    for (size_t i = 0; i < object->members_size; ++i)
    {
        if (strcmp(member_name, object->members[i].name.data) == 0)
        {
            if (target) target = &object->members[i];

            return 1;
        }
    }

    return 0;
}
member_t* core_object_member_get(object_t* object, const char* member_name)
{
    if (!object) return NULL;
    if (!object->members) return NULL;
    if (!member_name) return NULL;

    for (size_t i = 0; i < object->members_size; ++i)
    {
        if (strcmp(member_name, object->members[i].name.data) == 0)
        {
            return &object->members[i];
        }
    }

    return NULL;
}

int core_object_method_at(object_t* object, const char* method_name, method_t* target)
{
    if (!object) return 0;
    if (!object->methods) return 0;
    if (!method_name) return 0;

    for (size_t i = 0; i < object->methods_size; ++i)
    {
        if (strcmp(method_name, object->methods[i].name.data) == 0)
        {
            if (target) target = &object->methods[i];

            return 1;
        }
    }

    return 0;
}
method_t* core_object_method_get(object_t* object, const char* method_name)
{
    if (!object) return NULL;
    if (!object->methods) return NULL;
    if (!method_name) return NULL;

    for (size_t i = 0; i < object->methods_size; ++i)
    {
        if (strcmp(method_name, object->methods[i].name.data) == 0)
        {
            return &object->methods[i];
        }
    }

    return NULL;
}

void core_object_destroy(object_t* object)
{
    if (!object) return;

    if (object->name.data)
    {
        core_string_destroy(&object->name);
    }

    if (object->members)
    {
        for (size_t i = 0; i < object->members_size; ++i)
        {
            core_member_destroy(&object->members[i]);
        }

        free(object->members);
    }

    if (object->methods)
    {
        for (size_t i = 0; i < object->methods_size; ++i)
        {
            core_method_destroy(&object->methods[i]);
        }

        free(object->methods);
    }

    object->class = NULL;
    
    object->members = NULL;
    object->members_size = 0;

    object->methods = NULL;
    object->methods_size = 0;
}

void core_object_copy_callback(void* destination, const void* source)
{
    if (destination && source)
    {
        const object_t* src = (object_t*)source;

        object_t copy = {0};
        if (core_object_init(&copy, src->class, src->name.data, src->members_size, src->methods_size, src->members, src->methods))
        {
            memcpy(destination, &copy, sizeof(object_t));
        }
    }
}
void core_object_destroy_callback(void* object)
{
    if (object) core_object_destroy((object_t*)object);
}
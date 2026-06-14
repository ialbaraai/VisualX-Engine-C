#include "../../include/oop/core_class.h"

#include "../../include/oop/core_member.h"
#include "../../include/oop/core_method.h"

#include <stdlib.h>
#include <string.h>

int core_class_init(class_t* class, const class_t* base_class, const char* name, const size_t static_members_size, const size_t static_methods_size, const member_t* static_members, const method_t* static_methods, int(*constructor)(object_t* object, int argc, void** argv), void(*destructor)(object_t* object))
{
    if (!class) return 0;
    if (class->static_members || class->static_methods) return 0;
    if (!name) return 0;

    if (class->name.data)
    {
        core_string_set(&class->name, name);
    }
    else
    {
        core_string_init_data(&class->name, strlen(name) + 1, name);
    }

    class->base_class = base_class;

    class->static_members_size = static_members_size;
    class->static_members = malloc(static_members_size * sizeof(member_t));

    class->static_methods_size = static_methods_size;
    class->static_methods = malloc(static_methods_size * sizeof(method_t));

    if (!class->static_members)
    {
        class->static_members_size = 0;

        return 0;
    }

    if (!class->static_methods)
    {
        class->static_methods_size = 0;

        return 0;
    }

    for (size_t i = 0; i < class->static_members_size; ++i)
    {
        core_member_copy_callback(&class->static_members[i], &static_members[i]);
    }
    for (size_t i = 0; i < class->static_methods_size; ++i)
    {
        core_method_copy_callback(&class->static_methods[i], &static_methods[i]);
    }

    class->constructor = constructor;
    class->destructor = destructor;

    return 1;
}

int core_class_call(class_t* class, const char* static_method_name, int argc, void** argv)
{
    if (!class) return 0;
    if (!static_method_name) return 0;

    class_t* curclass = class;

    while (curclass != NULL)
    {
        for (size_t i = 0; i < curclass->static_methods_size; ++i)
        {
            if (strcmp(static_method_name, curclass->static_methods[i].name.data) == 0)
            {
                curclass->static_methods[i].function(NULL, argc, argv);

                return 1;
            }
        }

        curclass = curclass->base_class;
    }

    return 0;
}

int core_class_static_member_at(class_t* class, const char* static_member_name, member_t* target)
{
    if (!class) return 0;
    if (!static_member_name) return 0;

    class_t* curclass = class;

    while (curclass != NULL)
    {
        for (size_t i = 0; i < curclass->static_members_size; ++i)
        {
            if (strcmp(static_member_name, curclass->static_members[i].name.data) == 0)
            {
                if (target) target = &curclass->static_members[i];

                return 1;
            }
        }

        curclass = curclass->base_class;
    }

    return 0;
}
member_t* core_class_static_member_get(class_t* class, const char* static_member_name)
{
    if (!class) return NULL;
    if (!static_member_name) return NULL;

    class_t* curclass = class;

    while (curclass != NULL)
    {
        for (size_t i = 0; i < curclass->static_members_size; ++i)
        {
            if (strcmp(static_member_name, curclass->static_members[i].name.data) == 0)
            {
                return &curclass->static_members[i];
            }
        }

        curclass = curclass->base_class;
    }

    return NULL;
}

int core_class_static_method_at(class_t* class, const char* static_method_name, method_t* target)
{
    if (!class) return 0;
    if (!static_method_name) return 0;

    class_t* curclass = class;

    while (curclass != NULL)
    {
        for (size_t  i = 0; i < curclass->static_methods_size; ++i)
        {
            if (strcmp(static_method_name, curclass->static_methods[i].name.data) == 0)
            {
                if (target) target = &curclass->static_methods[i];

                return 1;
            }
        }

        curclass = curclass->base_class;
    }

    return 0;
}
method_t* core_class_static_method_get(class_t* class, const char* static_method_name)
{
    if (!class) return NULL;
    if (!static_method_name) return NULL;

    class_t* curclass = class;

    while (curclass != NULL)
    {
        for (size_t i = 0; i < curclass->static_methods_size; ++i)
        {
            if (strcmp(static_method_name, curclass->static_methods[i].name.data) == 0)
            {
                return &curclass->static_methods[i];
            }
        }

        curclass = curclass->base_class;
    }

    return NULL;
}

void core_class_destroy(class_t* class)
{
    if (!class) return;

    if (class->name.data)
    {
        core_string_destroy(&class->name);
    }

    if (class->static_members)
    {
        for (size_t i = 0; i < class->static_members_size; ++i)
        {
            core_member_destroy(&class->static_members[i]);
        }

        free(class->static_members);
    }
    if (class->static_methods)
    {
        for (size_t i = 0; i < class->static_methods_size; ++i)
        {
            core_method_destroy(&class->static_methods[i]);
        }

        free(class->static_methods);
    }

    class->base_class = NULL;

    class->static_members_size = 0;
    class->static_members = NULL;

    class->static_methods_size = 0;
    class->static_methods = NULL;

    class->constructor = NULL;
    class->destructor = NULL;
}

void core_class_copy_callback(void* destination, const void* source)
{
    if (destination && source)
    {
        const class_t* src = (class_t*)source;

        class_t copy = {0};
        if (core_class_init(&copy, src->base_class, src->name.data, src->static_members_size, src->static_methods_size, src->static_members, src->static_methods, src->constructor, src->destructor))
        {
            memcpy(destination, &copy, sizeof(class_t));
        }
    }
}
void core_class_destroy_callback(void* object)
{
    if (object) core_class_destroy((class_t*)object);
}
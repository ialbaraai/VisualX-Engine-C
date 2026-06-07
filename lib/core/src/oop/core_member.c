#include "../../include/oop/core_member.h"

#include <stdlib.h>
#include <string.h>

int core_member_init(member_t* member, const char* name, const size_t value_size, const visibility_t visibility, copy_function_t copy_function, destroy_function_t destroy_function)
{
    if (!member) return 0;
    if (member->value) return 0;
    if (!name) return 0;
    
    if (member->name.data)
    {
        core_string_set(&member->name, name);
    }
    else
    {
        core_string_init_data(&member->name, strlen(name) + 1, name);
    }

    member->size = value_size;

    member->value = malloc(value_size);
    member->visibility = visibility;

    if (!member->value)
    {
        member->size = 0;

        return 0;
    }

    member->copy_function = copy_function;
    member->destroy_function = destroy_function;

    return 1;
}
int core_member_init_data(member_t* member, const char* name, const size_t value_size, const visibility_t visibility, copy_function_t copy_function, destroy_function_t destroy_function, const void* value)
{
    int ret = core_member_init(member, name, value_size, visibility, copy_function, destroy_function);

    if (ret != 1) return ret;

    if (member->value && value)
    {
        if (member->copy_function)
        {
            member->copy_function(member->value, value);
        }
        else
        {
            memcpy(member->value, value, value_size);
        }

        return 1;
    }
    else
    {
        return 0;
    }
}

int core_member_set(member_t* member, const void* new_value)
{
    if (!member) return 0;
    if (!member->value) return 0;
    if (!new_value) return 0;

    if (member->destroy_function) member->destroy_function(member->value);

    if (member->copy_function)
    {
        member->copy_function(member->value, new_value);
    }
    else
    {
        memcpy(member->value, new_value, member->size);
    }

    return 1;
}

void core_member_destroy(member_t* member)
{
    if (!member) return;

    if (member->name.data)
    {
        core_string_destroy(&member->name);
    }

    if (member->value)
    {
        if (member->destroy_function)
        {
            member->destroy_function(member->value);
        }

        free(member->value);
    }

    member->value = NULL;

    member->size = 0;
    member->visibility = 0;

    member->copy_function = NULL;
    member->destroy_function = NULL;
}

void core_member_copy_callback(void* destination, const void* source)
{
    if (destination && source)
    {
        const member_t* src = (member_t*)source;

        member_t copy = {0};
        if (core_member_init_data(&copy, src->name.data, src->size, src->visibility, src->copy_function, src->destroy_function, src->value))
        {
            memcpy(destination, &copy, sizeof(member_t));
        }
    }
}
void core_member_destroy_callback(void* object)
{
    if (object) core_member_destroy((member_t*)object);
}
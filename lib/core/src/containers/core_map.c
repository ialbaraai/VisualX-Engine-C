#include "../../include/containers/core_map.h"

#include <stdlib.h>
#include <string.h>

#define MAP_MAX_SIZE (1024ULL * 1024ULL * 1024ULL)

int core_map_init(map_t* map, const size_t capacity, const size_t key_size, const size_t value_size, copy_function_t key_copy_function, destroy_function_t key_destroy_function, copy_function_t value_copy_function, destroy_function_t value_destroy_function, int (*compare_function)(const void* first, const void* second))
{
    if (!map) return 0;
    if (map->key_data || map->value_data) return 0;
    if (key_size == 0 || value_size == 0) return 0;

    if (capacity > MAP_MAX_SIZE / key_size) return 0;
    if (capacity > MAP_MAX_SIZE / value_size) return 0;

    map->capacity = capacity;
    map->size = 0;
    map->key_element_size = key_size;
    map->value_element_size = value_size;
    map->key_data = malloc(capacity * key_size);
    map->value_data = malloc(capacity * value_size);

    map->key_copy_function = key_copy_function;
    map->key_destroy_function = key_destroy_function;
    map->value_copy_function = value_copy_function;
    map->value_destroy_function = value_destroy_function;
    map->key_compare_function = compare_function;

    if (!map->key_data || !map->value_data)
    {
        map->capacity = 0;

        if (map->key_data)
        {
            free(map->key_data);
            map->key_data = NULL;
        }

        if (map->value_data)
        {
            free(map->value_data);
            map->value_data = NULL;
        }

        return 0;
    }

    return 1;
}
int core_map_init_data(map_t* map, const size_t capacity, const size_t key_size, const size_t value_size, const size_t count, copy_function_t key_copy_function, destroy_function_t key_destroy_function, copy_function_t value_copy_function, destroy_function_t value_destroy_function, int (*compare_function)(const void* first, const void* second), const void* key_data, const void* value_data)
{
    int ret = core_map_init(map, capacity, key_size, value_size, key_copy_function, key_destroy_function, value_copy_function, value_destroy_function, compare_function);

    if (ret != 1) return ret;

    if (map->key_data && map->value_data && key_data && value_data)
    {
        size_t actual_count = count > capacity ? capacity : count;

        if (map->key_copy_function)
        {
            for (size_t i = 0; i < actual_count; ++i)
            {
                map->key_copy_function((void*)((char*)map->key_data + (i * key_size)), (void*)((char*)key_data + (i * key_size)));
            }
        }
        else
        {
            memcpy(map->key_data, key_data, actual_count * key_size);
        }

        if (map->value_copy_function)
        {
            for (size_t i = 0; i < actual_count; ++i)
            {
                map->value_copy_function((void*)((char*)map->value_data + (i * value_size)),(void*)((char*)value_data + (i * value_size)));
            }
        }
        else
        {
            memcpy(map->value_data, value_data, actual_count * value_size);
        }

        map->size = actual_count;

        return 1;
    }
    else
    {
        return 0;
    }
}

int core_map_push_back(map_t* map, const void* key, const void* value)
{
    if (!map) return 0;
    if (!map->key_data || !map->value_data) return 0;
    if (!key || !value) return 0;

    if (map->size + 1 > map->capacity)
    {
        size_t new_capacity = map->capacity * 2 > map->size + 1 ? map->capacity * 2 : map->size + 1;

        if (new_capacity > MAP_MAX_SIZE / map->key_element_size) return 0;
        if (new_capacity > MAP_MAX_SIZE / map->value_element_size) return 0;

        void* key_temp = realloc(map->key_data, new_capacity * map->key_element_size);

        if (!key_temp) return 0;

        map->key_data = key_temp;

        void* value_temp = realloc(map->value_data, new_capacity * map->value_element_size);

        if (!value_temp) return 0;

        map->value_data = value_temp;
        map->capacity = new_capacity;
    }

    if (map->key_copy_function)
    {
        map->key_copy_function((void*)((char*)map->key_data + (map->size * map->key_element_size)), key);
    }
    else
    {
        memcpy((void*)((char*)map->key_data + (map->size * map->key_element_size)), key, map->key_element_size);
    }

    if (map->value_copy_function)
    {
        map->value_copy_function((void*)((char*)map->value_data + (map->size * map->value_element_size)), value);
    }
    else
    {
        memcpy((void*)((char*)map->value_data + (map->size * map->value_element_size)), value, map->value_element_size);
    }

    map->size++;
    return 1;
}
int core_map_pop_back(map_t* map)
{
    if (!map) return 0;
    if (!map->key_data || !map->value_data) return 0;
    if (map->size < 1) return 0;

    if (map->key_destroy_function)
    {
        map->key_destroy_function((void*)((char*)map->key_data + ((map->size - 1) * map->key_element_size)));
    }

    if (map->value_destroy_function)
    {
        map->value_destroy_function((void*)((char*)map->value_data + ((map->size - 1) * map->value_element_size)));
    }

    map->size--;
    return 1;
}
int core_map_remove(map_t* map, const void* key)
{
    if (!map) return 0;
    if (!map->key_data || !map->value_data) return 0;
    if (!key) return 0;

    int idx = -1;

    for (size_t i = 0; i < map->size; ++i)
    {
        void* current_key = (void*)((char*)map->key_data + (i * map->key_element_size));

        if (map->key_compare_function)
        {
            if (map->key_compare_function(current_key, key) == 0)
            {
                idx = i;
                break;
            }
        }
        else
        {
            if (memcmp(current_key, key, map->key_element_size) == 0)
            {
                idx = i;
                break;
            }
        }
    }

    if (idx == -1) return 0;

    if (map->key_destroy_function)
    {
        map->key_destroy_function((void*)((char*)map->key_data + (idx * map->key_element_size)));
    }
    if (map->value_destroy_function)
    {
        map->value_destroy_function((void*)((char*)map->value_data + (idx * map->value_element_size)));
    }

    if (idx != map->size - 1)
    {
        memcpy((void*)((char*)map->key_data + (idx * map->key_element_size)), (void*)((char*)map->key_data + ((map->size - 1) * map->key_element_size)), map->key_element_size);
        memcpy((void*)((char*)map->value_data + (idx * map->value_element_size)), (void*)((char*)map->value_data + ((map->size - 1) * map->value_element_size)), map->value_element_size);
    }

    map->size--;
    return 1;
}
int core_map_set(map_t* map, const void* key, const void* new_value)
{
    if (!map) return 0;
    if (!map->key_data || !map->value_data) return 0;
    if (!key || !new_value) return 0;

    for (size_t i = 0; i < map->size; ++i)
    {
        void* current_key = (void*)((char*)map->key_data + (i * map->key_element_size));
        void* current_value = (void*)((char*)map->value_data + (i * map->value_element_size));

        if (map->key_compare_function)
        {
            if (map->key_compare_function(current_key, key) == 0)
            {
                if (map->value_destroy_function)
                {
                    map->value_destroy_function(current_value);
                }

                if (map->value_copy_function)
                {
                    map->value_copy_function(current_value, new_value);
                }
                else
                {
                    memcpy(current_value, new_value, map->value_element_size);
                }

                return 1;
            }
        }
        else
        {
            if (memcmp(current_key, key, map->key_element_size) == 0)
            {
                if (map->value_destroy_function)
                {
                    map->value_destroy_function(current_value);
                }

                if (map->value_copy_function)
                {
                    map->value_copy_function(current_value, new_value);
                }
                else
                {
                    memcpy(current_value, new_value, map->value_element_size);
                }

                return 1;
            }
        }
    }

    return 0;
}
int core_map_clear(map_t* map)
{
    if (!map) return 0;
    if (!map->key_data || !map->value_data) return 0;

    if (map->key_destroy_function)
    {
        for (size_t i = 0; i < map->size; ++i)
        {
            map->key_destroy_function((void*)((char*)map->key_data + (i * map->key_element_size)));
        }
    }

    if (map->value_destroy_function)
    {
        for (size_t i = 0; i < map->size; ++i)
        {
            map->value_destroy_function((void*)((char*)map->value_data + (i * map->value_element_size)));
        }
    }

    map->size = 0;
    return 1;
}

int core_map_foreach(map_t* map, void(*function)(void* key, void* value))
{
    if (!map) return 0;
    if (!map->key_data || !map->value_data) return 0;
    if (!function) return 0;

    for (size_t i = 0; i < map->size; ++i)
    {
        function((void*)((char*)map->key_data + (i * map->key_element_size)), (void*)((char*)map->value_data + (i * map->value_element_size)));   
    }

    return 1;
}

void* core_map_first_key(const map_t* map)
{
    if (!map) return NULL;
    if (!map->key_data) return NULL;

    return map->key_data;
}
void* core_map_last_key(const map_t* map)
{
    if (!map) return NULL;
    if (!map->key_data) return NULL;

    return (void*)((char*)map->key_data + ((map->size - 1) * map->key_element_size));
}

void* core_map_first_value(const map_t* map)
{
    if (!map) return NULL;
    if (!map->value_data) return NULL;

    return map->value_data;
}
void* core_map_last_value(const map_t* map)
{
    if (!map) return NULL;
    if (!map->value_data) return NULL;

    return (void*)((char*)map->value_data + ((map->size - 1) * map->value_element_size));
}

int core_map_at(const map_t* map, const void* key, const void* target)
{
    if (!map) return 0;
    if (!map->key_data || !map->value_data) return 0;
    if (!key) return 0;

    for (size_t i = 0; i < map->size; ++i)
    {
        void* current_key = (void*)((char*)map->key_data + (i * map->key_element_size));
        void* current_value = (void*)((char*)map->value_data + (i * map->value_element_size));

        if (map->key_compare_function)
        {
            if (map->key_compare_function(current_key, key))
            {
                target = current_value;

                return 1;
            }
        }
        else
        {
            if (memcmp(current_key, key, map->key_element_size) == 0)
            {
                target = current_value;

                return 1;
            }
        }
    }

    return 0;
}
void* core_map_get(const map_t* map, const void* key)
{
    if (!map) return NULL;
    if (!map->key_data || !map->value_data) return NULL;
    if (!key) return NULL;

    for (size_t i = 0; i < map->size; ++i)
    {
        void* current_key = (void*)((char*)map->key_data + (i * map->key_element_size));
        void* current_value = (void*)((char*)map->value_data + (i * map->value_element_size));

        if (map->key_compare_function)
        {
            if (map->key_compare_function(current_key, key))
            {
                return current_value;
            }
        }
        else
        {
            if (memcmp(current_key, key, map->key_element_size) == 0)
            {
                return current_value;
            }
        }
    }

    return NULL;
}

void core_map_destroy(map_t* map)
{
    if (!map) return;

    if (map->key_data)
    {
        if (map->key_destroy_function)
        {
            for (size_t i = 0; i < map->size; ++i)
            {
                map->key_destroy_function((void*)((char*)map->key_data + (i * map->key_element_size)));
            }
        }

        free (map->key_data);
    }
    
    if (map->value_data)
    {
        if (map->value_destroy_function)
        {
            for (size_t i = 0; i < map->size; ++i)
            {
                map->value_destroy_function((void*)((char*)map->value_data + (i * map->value_element_size)));
            }
        }

        free(map->value_data);
    }

    map->key_data = NULL;
    map->value_data = NULL;

    map->key_element_size = 0;
    map->value_element_size = 0;

    map->capacity = 0;
    map->size = 0;

    map->key_copy_function = NULL;
    map->key_destroy_function = NULL;

    map->value_copy_function = NULL;
    map->value_destroy_function = NULL;

    map->key_compare_function = NULL;
}

void core_map_copy_callback(void* destination, const void* source)
{
    if (destination && source)
    {
        const map_t* src = (map_t*)source;

        map_t copy = {0};
        if (core_map_init_data(&copy, src->capacity, src->key_element_size, src->value_element_size, src->size, src->key_copy_function, src->key_destroy_function, src->value_copy_function, src->value_destroy_function, src->key_compare_function, src->key_data, src->value_data))
        {
            memcpy(destination, &copy, sizeof(map_t));
        }
    }
}
void core_map_destroy_callback(void* object)
{
    if (object) core_map_destroy((map_t*)object);
}
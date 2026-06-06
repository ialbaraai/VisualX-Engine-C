#ifndef CORE_MAP_H

#define CORE_MAP_h

#include <stddef.h>
#include "../core_backend.h"

typedef struct CORE_MAP_STRUCT
{
    void* key_data;
    void* value_data;
    size_t key_element_size;
    size_t value_element_size;
    size_t capacity;
    size_t size;
    void (*key_copy_function)(void* destination, const void* source);
    void (*key_destroy_function)(void* object);
    void (*value_copy_function)(void* destination, const void* source);
    void (*value_destroy_function)(void* object);
    int (*key_compare_function)(const void* first, const void* second);
} map_t;

int core_map_init(map_t* map, const size_t capacity, const size_t key_size, const size_t value_size, copy_function_t key_copy_function, destroy_function_t key_destroy_function, copy_function_t value_copy_function, destroy_function_t value_destroy_function, int (*compare_function)(const void* first, const void* second));
int core_map_init_data(map_t* map, const size_t capacity, const size_t key_size, const size_t value_size, const size_t count, copy_function_t key_copy_function, destroy_function_t key_destroy_function, copy_function_t value_copy_function, destroy_function_t value_destroy_function, int (*compare_function)(const void* first, const void* second), const void* key_data, const void* value_data);

int core_map_push_back(map_t* map, const void* key, const void* value);
int core_map_pop_back(map_t* map);
int core_map_remove(map_t* map, const void* key);
int core_map_set(map_t* map, const void* key, const void* new_value);
int core_map_clear(map_t* map);

int core_map_foreach(map_t* map, void(*function)(void* key, void* value));

void* core_map_first_key(const map_t* map);
void* core_map_last_key(const map_t* map);

void* core_map_first_value(const map_t* map);
void* core_map_last_value(const map_t* map);

int core_map_at(const map_t* map, const void* key, const void* target);
void* core_map_get(const map_t* map, const void* key);

void core_map_destroy(map_t* map);

void core_map_copy_callback(void* destination, const void* source);
void core_map_destroy_callback(void* object);

#endif
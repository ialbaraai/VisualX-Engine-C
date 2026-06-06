#ifndef CORE_VECTOR_H

#define CORE_VECTOR_H

#include <stddef.h>
#include "../core_backend.h"

typedef struct CORE_VECTOR_STRUCT
{
	void* data;
	size_t element_size;
	size_t size;
	size_t capacity;
	void (*copy_function)(void* destination, const void* source);
	void (*destroy_function)(void* object);
} vector_t;

int core_vector_init(vector_t* vector, const size_t capacity, const size_t element_size, copy_function_t copy_function, destroy_function_t destroy_function);
int core_vector_init_data(vector_t* vector, const size_t capacity, const size_t element_size, copy_function_t copy_function, destroy_function_t destroy_function, const void* data);

int core_vector_push_back(vector_t* vector, const void* value);
int core_vector_pop_back(vector_t* vector);
int core_vector_remove(vector_t* vector, const size_t index);
int core_vector_set(vector_t* vector, const size_t index, const void* value);
int core_vector_clear(vector_t* vector);

int core_vector_foreach(vector_t* vector, void(*function)(const size_t index, void* data));

void* core_vector_first(const vector_t* vector);
void* core_vector_last(const vector_t* vector);
int core_vector_at(const vector_t* vector, const size_t index, void* target);
void* core_vector_get(const vector_t* vector, const size_t index);

void core_vector_destroy(vector_t* vector);

void core_vector_copy_callback(void* destination, const void* source);
void core_vector_destroy_callback(void* object);

#endif

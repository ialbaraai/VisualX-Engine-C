#include "../../include/containers/core_vector.h"

#include <stdlib.h>
#include <string.h>

#define VECTOR_MAX_SIZE (1024ULL * 1024ULL * 1024ULL)

int core_vector_init(vector_t* vector, const size_t capacity, const size_t element_size, copy_function_t copy_function, destroy_function_t destroy_function)
{
	if (!vector) return 0;
	if (vector->data) return 0;
	
	vector->capacity = capacity;
	vector->size = 0;
	vector->element_size = element_size;
	vector->data = malloc(element_size * capacity);

	if (!vector->data)
	{
		vector->capacity = 0;
		
		return 0;
	}

	vector->copy_function = copy_function;
	vector->destroy_function = destroy_function;

	return 1;
}

int core_vector_init_data(vector_t* vector, const size_t capacity, const size_t element_size, copy_function_t copy_function, destroy_function_t destroy_function, const void* data)
{
	int ret = core_vector_init(vector, capacity, element_size, copy_function, destroy_function);

	if (ret != 1) return ret;

	if (vector->data && data)
	{
		if (vector->copy_function)
		{
			for (size_t i = 0; i < capacity; ++i)
			{
				vector->copy_function((void*)((char*)vector->data + (i * element_size)), (void*)((char*)data + (i * element_size)));
			}
		}
		else
		{
			memcpy(vector->data, data, capacity * element_size);
		}
		
		vector->size = capacity;
		
		return 1;
	}
	else
	{
		return 0;
	}
}

int core_vector_push_back(vector_t* vector, const void* value)
{
	if (!vector) return 0;
	if (!vector->data) return 0;
	if (!value) return 0;

	if (vector->size + 1 > vector->capacity)
	{
		size_t new_capacity = vector->capacity * 2 > vector->size + 1 ? vector->capacity * 2 : vector->size + 1;
	
		if (new_capacity * vector->element_size > VECTOR_MAX_SIZE) return 0;

		void* temp = realloc(vector->data, new_capacity * vector->element_size);

		if (!temp) return 0;

		vector->data = temp;
		vector->capacity = new_capacity;
	}
	
	if (vector->copy_function)
	{
		vector->copy_function((void*)((char*)vector->data + (vector->size * vector->element_size)), value);
	}
	else
	{
		memcpy((void*)((char*)vector->data + (vector->size * vector->element_size)), value, vector->element_size);
	}

	vector->size++;
	return 1;
}
int core_vector_pop_back(vector_t* vector)
{
	if (!vector) return 0;
	if (!vector->data) return 0;
	if (vector->size < 1) return 0;

	if (vector->destroy_function)
	{
		vector->destroy_function((void*)((char*)vector->data + ((vector->size - 1) * vector->element_size)));
	}

	vector->size--;
	return 1;
}
int core_vector_set(vector_t* vector, const size_t index, const void* value)
{
	if (!vector) return 0;
	if (!vector->data) return 0;
	if (!value) return 0;
	if (index >= vector->size) return 0;

	if (vector->destroy_function)
	{
		vector->destroy_function((void*)((char*)vector->data + (index * vector->element_size)));
	}

	if (vector->copy_function)
	{
		vector->copy_function((void*)((char*)vector->data + (index * vector->element_size)), value);
	}
	else
	{
		memcpy((void*)((char*)vector->data + (index * vector->element_size)), value, vector->element_size);
	}

	return 1;
}
int core_vector_remove(vector_t* vector, const size_t index)
{
	if (!vector) return 0;
	if (!vector->data) return 0;
	if (index >= vector->size) return 0;

	if (vector->destroy_function)
	{
		vector->destroy_function((void*)((char*)vector->data + (index * vector->element_size)));
	}

	if (index != vector->size - 1)
	{
		memcpy((void*)((char*)vector->data + (index * vector->element_size)), (void*)((char*)vector->data + ((vector->size - 1) * vector->element_size)), vector->element_size);
	}

	vector->size--;
	return 1;
}
int core_vector_clear(vector_t* vector)
{
	if (!vector) return 0;
	if (!vector->data) return 0;

	if (vector->destroy_function)
	{
		for (size_t i = 0; i < vector->size; ++i)
		{
			vector->destroy_function((void*)((char*)vector->data + (i * vector->element_size)));
		}
	}

	vector->size = 0;
	return 1;
}

int core_vector_foreach(vector_t* vector, void(*function)(const size_t index, void* data))
{
	if (!vector) return 0;
	if (!vector->data) return 0;
	if (!function) return 0;

	for (size_t i = 0; i < vector->size; ++i)
	{
		function(i, (void*)((char*)vector->data + (i * vector->element_size)));
	}

	return 1;
}

void* core_vector_first(const vector_t* vector)
{
	if (!vector) return NULL;
	if (!vector->data) return NULL;

	return vector->data;
}
void* core_vector_last(const vector_t* vector)
{
	if (!vector) return NULL;
	if (!vector->data) return NULL;
	if (vector->size == 0 ) return NULL;

	return (void*)((char*)vector->data + ((vector->size - 1) * vector->element_size));
}
int core_vector_at(const vector_t* vector, const size_t index, void* target)
{
	if (!vector) return 0;
	if (!vector->data) return 0;
	if (index >= vector->size) return 0;

	memcpy(target, (void*)((char*)vector->data + (index * vector->element_size)), vector->element_size);
	return 1;
}
void* core_vector_get(const vector_t* vector, const size_t index)
{
	if (!vector) return NULL;
	if (!vector->data) return NULL;
	if (index >= vector->size) return NULL;

	return (void*)((char*)vector->data + (index * vector->element_size));
}

void core_vector_destroy(vector_t* vector)
{
	if (!vector) return;
	if (vector->data)
	{
		if (vector->destroy_function)
		{
			for (size_t i = 0; i < vector->size; ++i)
			{
				vector->destroy_function((void*)((char*)vector->data + (i * vector->element_size)));
			}
		}
		
		free(vector->data);
	}
	
	vector->data = NULL;
	vector->element_size = 0;
	
	vector->capacity = 0;
	vector->size = 0;

	vector->copy_function = NULL;
	vector->destroy_function = NULL;
}

void core_vector_copy_callback(void* destination, const void* source)
{
	if (source && destination)
	{
		const vector_t* src = (vector_t*)source;

		vector_t copy = {0};
		if (core_vector_init_data(&copy, src->capacity, src->element_size, src->copy_function, src->destroy_function, src->data))
		{
			memcpy(destination, &copy, sizeof(vector_t));
		}
	}
}
void core_vector_destroy_callback(void* object)
{
	if (object) core_vector_destroy((vector_t*)object);
}
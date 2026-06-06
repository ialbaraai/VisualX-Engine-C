#ifndef CORE_STRING_H

#define CORE_STRING_H

#include <stddef.h>

typedef struct CORE_STRING_STRUCT
{
    char* data;
    size_t size;
    size_t capacity;
} string_t;

int core_string_init(string_t* string, const size_t capacity);
int core_string_init_data(string_t* string, const size_t capacity, const char* data);

int core_string_set(string_t* string, const char* data);
int core_string_copy(string_t* destination, const string_t* source);
int core_string_compare(const string_t* first, const string_t* second);

int core_string_pop_back(string_t* string);

int core_string_append_char(string_t* string, const char character);
int core_string_append_cstr(string_t* string, const char* data);
int core_string_append_string(string_t* string, const string_t* data);

int core_string_input(string_t* string);
int core_string_input_strict(string_t* string, const size_t maximum);

int core_string_upper(string_t* string);
int core_string_lower(string_t* string);

char core_string_at(const string_t* string, const size_t index);
int core_string_find(const string_t* string, const char character, size_t* index);

int core_string_replace_at(string_t* string, const size_t index, const char character);
int core_string_replace(string_t* string, const char oldc, const char newc);

void core_string_destroy(string_t* string);

void core_string_copy_callback(void* destination, const void* source);
void core_string_destroy_callback(void* object);
int core_string_compare_callback(const void* first, const void* second);

#endif
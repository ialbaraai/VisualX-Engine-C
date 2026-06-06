#ifndef CORE_FILE_H

#define CORE_FILE_H

#include <stddef.h>
#include "../core_backend.h"

#include "core_string.h"
#include "../containers/core_vector.h"

typedef struct CORE_FILE_STRUCT
{
    string_t filename;
    vector_t content;
    size_t initial_line_size;
} file_t;

int core_file_init(file_t* file, const char* filename, const size_t initial_lines_size, const size_t initial_line_size);

int core_file_read(file_t* file);
int core_file_write(file_t* file);

int core_file_set_filename(file_t* file, const char* new_filename);

void core_file_destroy(file_t* file);

void core_file_copy_callback(void* destination, const void* source);
void core_file_destroy_callback(void* object);

#endif
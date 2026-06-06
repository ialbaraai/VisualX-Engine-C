#include "../../include/core/core_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int core_file_init(file_t* file, const char* filename, const size_t initial_lines_size, const size_t initial_line_size)
{
    if (!file) return 0;
    if (file->filename.data || file->content.data) return 0;
    if (!filename) return 0;

    if (core_string_init_data(&file->filename, strlen(filename) + 1, filename) != 1)
    {
        return 0;
    }

    if (core_vector_init(&file->content, initial_lines_size, sizeof(string_t), core_string_copy_callback, core_string_destroy_callback) != 1)
    {
        return 0;
    }

    file->initial_line_size = initial_line_size;
    
    return 1;
}

int core_file_read(file_t* file)
{
    if (!file) return 0;
    if (!file->filename.data) return 0;
    if (!file->content.data) return 0;

    FILE* cfile = fopen(file->filename.data, "r");

    if (!cfile) return 0;

    core_vector_clear(&file->content);

    string_t curline = {0};
    core_string_init(&curline, file->initial_line_size);

    int curchar = 0;

    while ((curchar = fgetc(cfile)) != EOF)
    {
        if (curchar == '\n')
        {
            core_vector_push_back(&file->content, &curline);
            core_string_set(&curline, "");
        }
        else
        {
            core_string_append_char(&curline, (char)curchar);
        }
    }

    if (curline.size > 0) core_vector_push_back(&file->content, &curline);

    core_string_destroy(&curline);
    fclose(cfile);

    return 1;
}
int core_file_write(file_t* file)
{
    if (!file) return 0;
    if (!file->content.data) return 0;
    if (!file->filename.data) return 0;

    FILE* cfile = fopen(file->filename.data, "w");

    if (!cfile) return 0;

    for (size_t i = 0; i < file->content.size; ++i)
    {
        if (i > 0) fputc('\n', cfile);
        fprintf(cfile, "%s", ((string_t*)core_vector_get(&file->content, i))->data);
    }

    fclose(cfile);

    return 1;
}

int core_file_set_filename(file_t* file, const char* new_filename)
{
    if (!file) return 0;
    if (!file->filename.data) return 0;
    if (!new_filename) return 0;

    return core_string_set(&file->filename, new_filename);
}

void core_file_destroy(file_t* file)
{
    if (!file) return;

    core_vector_destroy(&file->content);
    core_string_destroy(&file->filename);
    
    file->initial_line_size = 0;
}

void core_file_copy_callback(void* destination, const void* source)
{
    if (destination && source)
    {
        const file_t* src = (file_t*)source;

        file_t copy = {0};
        if (core_file_init(&copy, src->filename.data, src->content.size, src->initial_line_size))
        {
            core_vector_copy_callback(&copy.content, &src->content);
            memcpy(destination, &copy, sizeof(size_t));
        }
    }
}
void core_file_destroy_callback(void* object)
{
    if (object) core_file_destroy((file_t*)object);
}
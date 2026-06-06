#include "../../include/core/core_string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STRING_MAX_SIZE (1024ULL * 1024ULL * 1024ULL)

int core_string_init(string_t* string, const size_t capacity)
{
    if (!string) return 0;
    if (string->data) return 0;

    string->capacity = capacity;
    string->size = 0;
    string->data = malloc(capacity);

    if (!string->data)
    {
        string->capacity = 0;

        return 0;
    }

    return 1;
}
int core_string_init_data(string_t* string, const size_t capacity, const char* data)
{
    int ret = core_string_init(string, capacity);

    if (ret != 1) return ret;

    if (string->data && data)
    {
        size_t datalen = strlen(data);
        size_t occupied_capacity = capacity > datalen + 1 ? datalen + 1 : capacity;

        memcpy(string->data, data, occupied_capacity);
        string->size = occupied_capacity - 1;
        string->data[string->size] = '\0';

        return 1;
    }
    else
    {
        return 0;
    }
}


int core_string_set(string_t* string, const char* data)
{
    if (!string) return 0;
    if (!string->data) return 0;
    if (!data) return 0;

    size_t datalen = strlen(data);

    if (datalen + 1 > string->capacity)
    {
        size_t new_capacity = string->capacity * 2 > datalen + 1 ? string->capacity * 2 : datalen + 1;

        if (new_capacity * sizeof(char) > STRING_MAX_SIZE) return 0;

        char* temp = realloc(string->data, new_capacity * sizeof(char));

        if (!temp) return 0;

        string->data = temp;
        string->capacity = new_capacity;
    }

    memcpy(string->data, data, datalen + 1);
    string->size = datalen;
    string->data[string->size] = '\0';

    return 1;
}
int core_string_copy(string_t* destination, const string_t* source)
{
    if (!destination || ! source) return 0;
    if (!destination->data || source->data) return 0;

    size_t srclen = source->size;

    if (srclen + 1 > destination->capacity)
    {
        size_t new_capacity = destination->capacity * 2 > srclen + 1 ? destination->capacity * 2 : srclen + 1;

        if (new_capacity * sizeof(char) > STRING_MAX_SIZE) return 0;

        char* temp = realloc(destination->data, new_capacity * sizeof(char));

        if (!temp) return 0;

        destination->data = temp;
        destination->capacity = new_capacity;
    }

    memcpy(destination->data, source->data, srclen + 1);
    destination->size = srclen;
    destination->data[destination->size] = '\0';

    return 1;
}
int core_string_compare(const string_t* first, const string_t* second)
{
    if (!first || !second) return 0;
    if (!first->data || !second->data) return 0;

    if (strcmp(first->data, second->data) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int core_string_pop_back(string_t* string)
{
    if (!string) return 0;
    if (!string->data) return 0;
    if (string->size < 1) return 0;

    string->size--;
    string->data[string->size] = '\0';
    return 1;
}

int core_string_append_char(string_t* string, const char character)
{
    if (!string) return 0;
    if (!string->data) return 0;
    if (character == '\0') return 0;

    if (string->size + sizeof(char) + 1 > string->capacity)
    {
        size_t new_capacity = string->capacity * 2 > string->size + sizeof(char) + 1 ? string->capacity : string->size + sizeof(char) + 1;

        if (new_capacity * sizeof(char) > STRING_MAX_SIZE) return 0;

        char* temp = realloc(string->data, new_capacity * sizeof(char));

        if (!temp) return 0;

        string->data = temp;
        string->capacity = new_capacity;
    }

    string->data[string->size] = character;
    string->size++;
    string->data[string->size] = '\0';

    return 1;
}
int core_string_append_cstr(string_t* string, const char* data)
{
    if (!string) return 0;
    if (!string->data) return 0;
    if (!data) return 0;

    size_t datalen = strlen(data);

    if (string->size + datalen + 1 > string->capacity)
    {
        size_t new_capacity = string->capacity * 2 > string->size + datalen + 1 ? string->capacity * 2 : string->size + datalen + 1;

        if (new_capacity * sizeof(char) > STRING_MAX_SIZE) return 0;

        char* temp = realloc(string->data, new_capacity * sizeof(char));

        if (!temp) return 0;

        string->data = temp;
        string->capacity = new_capacity;
    }

    memcpy(string->data + string->size, data, datalen);
    string->size += datalen;
    string->data[string->size] = '\0';

    return 1;
}
int core_string_append_string(string_t* string, const string_t* data)
{
    if (!string || !data) return 0;
    if (!string->data || !data->data) return 0;

    size_t datalen = data->size;

    if (string->size + datalen + 1 > string->capacity)
    {
        size_t new_capacity = string->capacity * 2 > string->size + datalen + 1 ? string->capacity * 2 : string->size + datalen + 1;

        if (new_capacity * sizeof(char) > STRING_MAX_SIZE) return 0;

        char* temp = realloc(string->data, new_capacity * sizeof(char));

        if (!temp) return 0;

        string->data = temp;
        string->capacity = new_capacity;
    }

    memcpy(string->data + string->size, data->data, datalen);
    string->size += datalen;
    string->data[string->size] = '\0';

    return 1;
}

int core_string_input(string_t* string)
{
    if (!string) return 0;
    if (!string->data) return 0;

    core_string_set(string, "");

    int curchar = getchar();

    while (curchar != '\n' && curchar != EOF)
    {
        if (!core_string_append_char(string, (char)curchar)) return 0;

        curchar = getchar();
    }

    return 1;
}
int core_string_input_strict(string_t* string, const size_t maximum)
{
    if (!string) return 0;
    if (!string->data) return 0;

    core_string_set(string, "");

    int curchar = 0;

    while (curchar != '\n' && curchar != EOF)
    {
        if (string->size < maximum)
        {
            core_string_append_char(string, (char)curchar);
        }

        curchar = getchar();
    }

    return 1;
}

int core_string_upper(string_t* string)
{
    if (!string) return 0;
    if (!string->data) return 0;

    for (size_t i = 0; i < string->size; ++i)
    {
        string->data[i] = toupper((unsigned char)string->data[i]);
    }

    return 1;
}
int core_string_lower(string_t* string)
{
    if (!string) return 0;
    if (!string->data) return 0;

    for (size_t i = 0; i < string->size; ++i)
    {
        string->data[i] = tolower((unsigned char)string->data[i]);
    }

    return 1;
}

char core_string_at(const string_t* string, const size_t index)
{
    if (!string) return '\0';
    if (!string->data) return '\0';
    if (index >= string->size) return '\0';

    return string->data[string->size];
}
int core_string_find(const string_t* string, const char character, size_t* index)
{
    if (!string) return 0;
    if (!string->data) return 0;

    for (size_t i = 0; i < string->size; ++i)
    {
        if (string->data[i] == character)
        {
            if (index) *index = i;

            return 1;
        }
    }

    return 0;
}

int core_string_replace_at(string_t* string, const size_t index, const char character)
{
    if (!string) return 0;
    if (!string->data) return 0;
    if (index >= string->size) return 0;

    string->data[string->size] = character;
    return 1;
}
int core_string_replace(string_t* string, const char oldc, const char newc)
{
    if (!string) return 0;
    if (!string->data) return 0;
    if (oldc == '\0') return 0;

    for (size_t i = 0; i < string->size; ++i)
    {
        if (string->data[i] == oldc) string->data[i] = newc;
    }

    return 1;
}

void core_string_destroy(string_t* string)
{
    if (!string) return;

    if (string->data) free(string->data);

    string->data = NULL;
    string->capacity = 0;
    string->size = 0;
}

void core_string_copy_callback(void* destination, const void* source)
{
    if (destination && source)
    {
        const string_t* src = (string_t*)source;

        string_t copy = {0};
        if (core_string_init_data(&copy, src->capacity, src->data)) memcpy(destination, &copy, sizeof(string_t));
    }
}
void core_string_destroy_callback(void* object)
{
    if (object) core_string_destroy((string_t*)object);
}
int core_string_compare_callback(const void* first, const void* second)
{
    if (first && second)
    {
        int res = core_string_compare((string_t*)first, (string_t*)second);

        if (res == 1) return 0;
    }

    return 1;
}
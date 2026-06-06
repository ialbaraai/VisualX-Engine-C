#ifndef CORE_BACKEND_H

#define CORE_BACKEND_H

typedef void(*copy_function_t)(void* destination, const void* source);
typedef void(*destroy_function_t)(void* object);

#endif

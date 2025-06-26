#ifndef PRIMROSE_MAIN_H
#define PRIMROSE_MAIN_H

#include <stdio.h>

#define __need_size_t
#include <stddef.h>

typedef enum
{
    PRIMROSE_VERBOSE_BEGIN,
    PRIMROSE_VERBOSE,
    PRIMROSE_VERBOSE_OK,
    PRIMROSE_LOG,
    PRIMROSE_SUCCESS,
    PRIMROSE_NOTE,
    PRIMROSE_WARNING,
    PRIMROSE_ERROR
} primrose_type_t;

typedef struct
{
    const char *const file;
    const char *const function;
    const size_t line;
} primrose_metadata_t;

#if BUILD_TYPE == 0
void(primrose_log)(primrose_metadata_t data, primrose_type_t type,
                   FILE *redirect, const char *const format, ...);

#define primrose_log(type, message, ...)                                       \
    primrose_log((primrose_metadata_t){FILENAME, __func__, __LINE__},          \
                 PRIMROSE_##type, nullptr, message __VA_OPT__(, ) __VA_ARGS__)
#else
#define primrose_log(type, message, ...)
#endif

#endif // PRIMROSE_MAIN_H

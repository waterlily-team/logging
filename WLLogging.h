#ifndef WLLOGGING_MAIN_H
#define WLLOGGING_MAIN_H

#include <stdio.h>

#define __need_size_t
#include <stddef.h>

typedef enum
{
    WATERLILY_LOG_TYPE_VERBOSE_BEGIN,
    WATERLILY_LOG_TYPE_VERBOSE,
    WATERLILY_LOG_TYPE_VERBOSE_OK,
    WATERLILY_LOG_TYPE_LOG,
    WATERLILY_LOG_TYPE_SUCCESS,
    WATERLILY_LOG_TYPE_NOTE,
    WATERLILY_LOG_TYPE_WARNING,
    WATERLILY_LOG_TYPE_ERROR
} waterlily_log_type_t;

typedef struct
{
    const char *const file;
    const char *const function;
    const size_t line;
} waterlily_log_metadata_t;

void(waterlily_log)(waterlily_log_metadata_t data, waterlily_log_type_t type,
                    FILE *redirect, const char *const format, ...);

#define waterlily_log(type, message, ...)                                      \
    waterlily_log((waterlily_log_metadata_t){FILENAME, __func__, __LINE__},    \
                  WATERLILY_LOG_TYPE_##type, nullptr,                          \
                  message __VA_OPT__(, ) __VA_ARGS__)

#endif // WLLOGGING_MAIN_H

#ifndef TKLOGGING_MAIN_H
#define TKLOGGING_MAIN_H

#include <stdio.h>

#define __need_size_t
#include <stddef.h>

typedef enum
{
    RPGTK_LOG_TYPE_VERBOSE_BEGIN,
    RPGTK_LOG_TYPE_VERBOSE,
    RPGTK_LOG_TYPE_VERBOSE_OK,
    RPGTK_LOG_TYPE_LOG,
    RPGTK_LOG_TYPE_SUCCESS,
    RPGTK_LOG_TYPE_WARNING,
    RPGTK_LOG_TYPE_ERROR
} rpgtk_log_type_t;

typedef struct
{
    const char *const file;
    const char *const function;
    const size_t line;
} rpgtk_log_metadata_t;

void(rpgtk_log)(rpgtk_log_metadata_t data, rpgtk_log_type_t type,
                FILE *redirect, const char *const format, ...);

#define rpgtk_log(type, message, ...)                                          \
    rpgtk_log((rpgtk_log_metadata_t){FILENAME, __func__, __LINE__},            \
              RPGTK_LOG_TYPE_##type, nullptr,                                  \
              message __VA_OPT__(, ) __VA_ARGS__)

#endif // TKLOGGING_MAIN_H

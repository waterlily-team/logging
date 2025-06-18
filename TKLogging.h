#ifndef TKLOGGING_MAIN_H
#define TKLOGGING_MAIN_H

#include <stdio.h>

typedef enum
{
    RPGTK_LOG_TYPE_VERBOSE,
    RPGTK_LOG_TYPE_INFORMATION,
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
    rpgtk_log((rpgtk_log_metadata_t){__FILE__, __func__, __LINE__}, type,      \
              nullptr, message __VA_OPT__(, ) __VA_ARGS__)

#endif // TKLOGGING_MAIN_H

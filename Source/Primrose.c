#include <Primrose.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

static FILE *getProperStream(primrose_type_t type)
{
    switch (type)
    {
        case PRIMROSE_VERBOSE_BEGIN: [[fallthrough]];
        case PRIMROSE_VERBOSE:       [[fallthrough]];
        case PRIMROSE_VERBOSE_OK:    [[fallthrough]];
        case PRIMROSE_LOG:           [[fallthrough]];
        case PRIMROSE_SUCCESS:       [[fallthrough]];
        case PRIMROSE_NOTE:          return stdout;
        case PRIMROSE_WARNING:       [[fallthrough]];
        case PRIMROSE_ERROR:         return stderr;
    }
    // We can't really ever get here, but GCC whines and it's also good
    // insurance if I forget to update the switch with any new log types.
    return stdout;
}

static const char *const tags[] = {
    [PRIMROSE_VERBOSE_BEGIN] = "STRT", [PRIMROSE_VERBOSE] = "VERB",
    [PRIMROSE_VERBOSE_OK] = "GOOD",    [PRIMROSE_LOG] = "INFO",
    [PRIMROSE_SUCCESS] = " OK ",       [PRIMROSE_NOTE] = "NOTE",
    [PRIMROSE_WARNING] = "WARN",       [PRIMROSE_ERROR] = "FAIL"};
static const uint8_t colors[] = {
    [PRIMROSE_VERBOSE_BEGIN] = 93, [PRIMROSE_VERBOSE] = 90,
    [PRIMROSE_VERBOSE_OK] = 32,    [PRIMROSE_LOG] = 0,
    [PRIMROSE_SUCCESS] = 92,       [PRIMROSE_NOTE] = 33,
    [PRIMROSE_WARNING] = 31,       [PRIMROSE_ERROR] = 91};

static int columnSize = 0;

void(primrose_log)(primrose_metadata_t data, primrose_type_t type,
                   FILE *redirect, const char *const format, ...)
{
    if (columnSize == 0)
    {
        struct winsize w_stdout, w_stderr;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w_stdout);
        ioctl(STDERR_FILENO, TIOCGWINSZ, &w_stderr);
        columnSize = (w_stdout.ws_col < w_stderr.ws_col ? w_stdout.ws_col
                                                        : w_stderr.ws_col);
    }

    if (redirect == nullptr) redirect = getProperStream(type);

    char message[columnSize];
    snprintf(message, columnSize,
             "[\033[%dm%s\033[0m] %-10s fn. %-20s ln. %04zu: %s \n",
             colors[type], tags[type], data.file, data.function, data.line,
             format);
    if (type == PRIMROSE_WARNING || type == PRIMROSE_ERROR)
    {
        char submessage[50];
        snprintf(submessage, 50,
                 "       > Current ERRNO (may be garbage): %d\n", errno);
        strncat(message, submessage, columnSize);
    }

    va_list args;
    va_start(args, format);
    vfprintf(redirect, message, args);
    va_end(args);
}

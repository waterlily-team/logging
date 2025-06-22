#include <WLLogging.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <unistd.h>

static FILE *getProperStream(waterlily_log_type_t type)
{
    switch (type)
    {
        case WATERLILY_LOG_TYPE_VERBOSE_BEGIN: [[fallthrough]];
        case WATERLILY_LOG_TYPE_VERBOSE:       [[fallthrough]];
        case WATERLILY_LOG_TYPE_VERBOSE_OK:    [[fallthrough]];
        case WATERLILY_LOG_TYPE_LOG:           [[fallthrough]];
        case WATERLILY_LOG_TYPE_SUCCESS:       return stdout;
        case WATERLILY_LOG_TYPE_WARNING:       [[fallthrough]];
        case WATERLILY_LOG_TYPE_ERROR:         return stderr;
    }
    // We can't really ever get here, but GCC whines and it's also good
    // insurance if I forget to update the switch with any new log types.
    return stdout;
}

static const char *const tags[] = {[WATERLILY_LOG_TYPE_VERBOSE_BEGIN] = "STRT",
                                   [WATERLILY_LOG_TYPE_VERBOSE] = "VERB",
                                   [WATERLILY_LOG_TYPE_VERBOSE_OK] = "GOOD",
                                   [WATERLILY_LOG_TYPE_LOG] = "INFO",
                                   [WATERLILY_LOG_TYPE_SUCCESS] = " OK ",
                                   [WATERLILY_LOG_TYPE_WARNING] = "WARN",
                                   [WATERLILY_LOG_TYPE_ERROR] = "FAIL"};
static const uint8_t colors[] = {
    [WATERLILY_LOG_TYPE_VERBOSE_BEGIN] = 93, [WATERLILY_LOG_TYPE_VERBOSE] = 90,
    [WATERLILY_LOG_TYPE_VERBOSE_OK] = 32,    [WATERLILY_LOG_TYPE_LOG] = 0,
    [WATERLILY_LOG_TYPE_SUCCESS] = 92,       [WATERLILY_LOG_TYPE_WARNING] = 33,
    [WATERLILY_LOG_TYPE_ERROR] = 31};

static int columnSize = 0;

void(waterlily_log)(waterlily_log_metadata_t data, waterlily_log_type_t type,
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
    if (type == WATERLILY_LOG_TYPE_WARNING || type == WATERLILY_LOG_TYPE_ERROR)
        snprintf(message, columnSize, "Errno: %d", errno);

    va_list args;
    va_start(args, format);
    vfprintf(redirect, message, args);
    va_end(args);
}

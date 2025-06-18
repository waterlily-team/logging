#include <TKLogging.h>
#include <stdarg.h>

static FILE *getProperStream(rpgtk_log_type_t type)
{
    switch (type)
    {
        case RPGTK_LOG_TYPE_VERBOSE:     [[fallthrough]];
        case RPGTK_LOG_TYPE_INFORMATION: return stdout;
        case RPGTK_LOG_TYPE_WARNING:     [[fallthrough]];
        case RPGTK_LOG_TYPE_ERROR:       return stderr;
    }
    // We can't really ever get here, but GCC whines and it's also good
    // insurance if I forget to update the switch with any new log types.
    return stdout;
}

void(rpgtk_log)(rpgtk_log_metadata_t data, rpgtk_log_type_t type,
                FILE *redirect, const char *const format, ...)
{
    if (redirect == nullptr) redirect = getProperStream(type);

    (void)data;

    va_list args;
    va_start(args, format);
    vfprintf(redirect, format, args);
    va_end(args);
}

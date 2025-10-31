#include "../include/trio/log.h"

const char* TrioLogLevelToString(TrioLogLevel logLevel, bool stylized) {
    if (stylized) {
        switch (logLevel) {
            case TRIO_INFO:  return "\x1b[37;1m[INFO]\x1b[0m";
            case TRIO_WARN:  return "\x1b[33;1m[WARN]\x1b[0m";
            case TRIO_ERROR: return "\x1b[31;1m[ERROR]\x1b[0m";
            case TRIO_FATAL: return "\x1b[31;1;7m[FATAL]\x1b[0m";
            default:         return "\x1b[35;1m[UNKNOWN]\x1b[0m";
        }
    }
    else {
        switch (logLevel) {
            case TRIO_INFO:  return "[INFO]";
            case TRIO_WARN:  return "[WARN]";
            case TRIO_ERROR: return "[ERROR]";
            case TRIO_FATAL: return "[FATAL]";
            default:         return "[UNKNOWN]";
        }
    }
}

char* TrioFormatStringVA(const char* fmt, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    char* buffer = malloc(len + 1);
    vsnprintf(buffer, len + 1, fmt, args);
    return buffer;
}

void TrioLog(const char* caller, TrioLogLevel logLevel, const char* fmt, ...) {

    va_list args;
    va_start(args, fmt);
    char* buffer = TrioFormatStringVA(fmt, args);
    va_end(args);

    printf("%s %s CALLER=%s\n", TrioLogLevelToString(logLevel, true), buffer, caller);
    free(buffer);
}
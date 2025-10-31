#pragma once

typedef enum TrioLogLevel {
    TRIO_INFO,
    TRIO_WARN,
    TRIO_ERROR,
    TRIO_FATAL,
} TrioLogLevel;

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

const char* TrioLogLevelToString(TrioLogLevel logLevel, bool stylized);
void TrioLog(const char* caller, TrioLogLevel logLevel, const char* fmt, ...);

#ifdef __cplusplus
}
#endif
#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum TrioLogLevel {
    TRIO_INFO,
    TRIO_WARN,
    TRIO_ERROR,
    TRIO_FATAL,
} TrioLogLevel;

typedef enum TrioLogOutputType {
    TRIO_STDOUT,
    TRIO_FILE,
    TRIO_STDOUT_AND_FILE,
} TrioLogOutputType;

typedef struct TrioLogConfig {
    bool stdoutStylized;
    bool fileStylized;
    TrioLogOutputType logOutputType;
    char* logOutputFilePath;
} TrioLogConfig;

#ifdef __cplusplus
extern "C" {
#endif

const char* TrioLogLevelToString(TrioLogLevel logLevel, bool stylized);
TrioLogConfig* TrioGetDefaultLogConfig();
void TrioLog(const char* caller, TrioLogConfig* logConfig, TrioLogLevel logLevel, const char* fmt, ...);

#ifdef __cplusplus
}
#endif
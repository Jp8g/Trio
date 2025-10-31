#pragma once

typedef enum LogLevel {
    INFO,
    WARN,
    ERROR,
    FATAL,
} LogLevel;

#ifdef __cplusplus
extern "C" {
#endif

const char* TrioLogLevelToString(LogLevel logLevel);
void TrioLog(LogLevel logLevel, const char* logText);

#ifdef __cplusplus
}
#endif
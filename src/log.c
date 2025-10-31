#include "../include/trio/log.h"
#include <stdio.h>

const char* TrioLogLevelToString(LogLevel logLevel) {
    switch (logLevel) {
        case INFO: return "INFO";
        case WARN: return "WARN";
        case ERROR: return "ERROR";
        case FATAL: return "FATAL";
    }
}

void TrioLog(LogLevel logLevel, const char* logText) {
    printf("[%s] %s", TrioLogLevelToString(logLevel), logText);
}
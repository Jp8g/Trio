#include "../include/log.h"
#include <unistd.h>

TrioLogConfig* defaultLogConfig;
char* cwd = NULL;

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
    if (!buffer) {
        fprintf(stderr, "\x1b[31;1m[ERROR]\x1b[0m Failed allocate memory (%d Bytes) for char* buffer CALLER=%s\n", len + 1, __func__);
        perror(NULL);
        return NULL;
    }

    vsnprintf(buffer, len + 1, fmt, args);
    return buffer;
}

char* TrioGetCurrentWorkingDirectory() {
    if (!cwd) {
        cwd = getcwd(NULL, 0);
    }
    
    return cwd;
}

TrioLogConfig* TrioGetDefaultLogConfig() {
    if (!defaultLogConfig) {
        defaultLogConfig = malloc(sizeof(TrioLogConfig));

        if (!defaultLogConfig) {
            return NULL;
        }

        defaultLogConfig->stdoutStylized = true;
        defaultLogConfig->fileStylized = true;
        defaultLogConfig->logOutputType = TRIO_STDOUT;
        defaultLogConfig->logOutputFilePath = NULL;
    }

    return defaultLogConfig;
}

void TrioLog(const char* caller, TrioLogConfig* logConfig, TrioLogLevel logLevel, const char* fmt, ...) {

    va_list args;
    va_start(args, fmt);
    char* fmtBuffer = TrioFormatStringVA(fmt, args);

    if (!fmtBuffer) {
        fprintf(stderr, "\x1b[31;1m[ERROR]\x1b[0m Failed to format log message \"%s\" CALLER=%s\n", fmt, __func__);
        perror(NULL);
        va_end(args);
        return;
    }

    va_end(args);

    if (logConfig->logOutputType == TRIO_STDOUT || logConfig->logOutputType == TRIO_STDOUT_AND_FILE) {
        printf("%s %s CALLER=%s\n", TrioLogLevelToString(logLevel, logConfig->stdoutStylized), fmtBuffer, caller);
    }

    if (logConfig->logOutputType == TRIO_FILE || logConfig->logOutputType == TRIO_STDOUT_AND_FILE) {

        FILE* fileptr = fopen(logConfig->logOutputFilePath, "a");

        if (fileptr) {
            if (fprintf(fileptr, "%s %s CALLER=%s\n", TrioLogLevelToString(logLevel, logConfig->fileStylized), fmtBuffer, caller) < 0) {
                char* cwd = getcwd(NULL, 0);
                fprintf(stderr, "\x1b[31;1m[ERROR]\x1b[0m Failed to write to file \"%s\" from working directory \"%s\" CALLER=%s\n", logConfig->logOutputFilePath, cwd ? cwd : "UNKNOWN" , __func__);
                if (cwd) free(cwd);
                perror(NULL);
            }
            fclose(fileptr);
        } else {
            char* cwd = getcwd(NULL, 0);
            fprintf(stderr, "\x1b[31;1m[ERROR]\x1b[0m Failed to open file \"%s\" from working directory \"%s\" CALLER=%s\n", logConfig->logOutputFilePath, cwd ? cwd : "UNKNOWN", __func__);
            if (cwd) free(cwd);
            perror(NULL);
        }
    }

    free(fmtBuffer);
}

void TrioShutdownLog() {
    free(cwd);
}
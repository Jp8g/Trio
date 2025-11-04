#include "../include/io.h"

char* TrioGetExePath() {
    int len = wai_getExecutablePath(NULL, 0, NULL);
    char* path = (char*)malloc(len + 1);
    wai_getExecutablePath(path, len + 1, NULL);
    path[len] = '\0';

    return path;
}

char* TrioGetParentDir(const char* path) {
    char* pathCopy = strdup(path);
    if (!pathCopy) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to duplicate string");
        return NULL;
    }

    char* parent = dirname(pathCopy);
    char* result = strdup(parent);

    free(pathCopy);
    return result;
}

char* TrioGetExeDir() {
    char* exePath = TrioGetExePath();
    char* path = TrioGetParentDir(exePath);
    free(exePath);

    return path;
}

char* TrioResolvePath(const char* path) {
    char* exeDir = TrioGetExeDir();

    int len = snprintf(NULL, 0, "%s/%s", exeDir, path);

    char* resolvedPath = malloc(len + 1);

    snprintf(resolvedPath, len + 1, "%s/%s", exeDir, path);

    free(exeDir);

    return resolvedPath;
}
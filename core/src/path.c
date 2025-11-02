#include "../include/path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* TrioGetExePath() {
    int len = wai_getExecutablePath(NULL, 0, NULL);
    char* path = (char*)malloc(len + 1);
    wai_getExecutablePath(path, len, NULL);
    path[len] = '\0';

    return path;
}

char* TrioGetParentDir(const char* path) {
    if (!path || !*path)
        return strdup("../");

    char* parentDir = strdup(path);
    size_t len = strlen(parentDir);

    if (len > 0 && parentDir[len - 1] == '/')
        parentDir[--len] = '\0';

    char* slash = strrchr(parentDir, '/');
    if (slash)
        *slash = '\0';
    else
        parentDir[0] = '\0';

    size_t newLen = strlen(parentDir);
    char* result = malloc(newLen + 2);
    strcpy(result, parentDir);
    result[newLen] = '/';
    result[newLen + 1] = '\0';

    free(parentDir);
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
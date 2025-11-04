#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "../../external/whereami/src/whereami.h"
#include "log.h"

#if __cplusplus
extern "C" {
#endif

char* TrioGetExePath();
char* TrioGetParentDir(const char* path);
char* TrioGetExeDir();
char* TrioResolvePath(const char* path);

#if __cplusplus
}
#endif
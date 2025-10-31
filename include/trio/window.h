#pragma once

#include <stdbool.h>
#include "../../external/glfw/include/GLFW/glfw3.h"

typedef GLFWwindow TrioWindow;

#ifdef __cplusplus
extern "C" {
#endif

TrioWindow* TrioCreateWindow(const char * Title, int Width, int Height);
int TrioGetWindowCount();
void TrioSetCurrentWindow(TrioWindow* window);
bool TrioWindowShouldClose(TrioWindow* window);
void TrioCloseWindow(TrioWindow* window);

#ifdef __cplusplus
}
#endif
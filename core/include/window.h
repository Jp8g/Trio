#pragma once

#include "../../external/glfw/include/GLFW/glfw3.h"
#include <stdbool.h>

typedef GLFWwindow TrioWindow;

#ifdef __cplusplus
extern "C" {
#endif

TrioWindow* TrioCreateWindow(const char * Title, int Width, int Height);
uint32_t TrioGetWindowCount();
void TrioSetCurrentWindow(TrioWindow* window);
bool TrioWindowShouldClose(TrioWindow* window);
void TrioCloseWindow(TrioWindow* window);

#ifdef __cplusplus
}
#endif
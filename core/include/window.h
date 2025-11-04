#pragma once

#define GLFW_INCLUDE_NONE

#include "../../external/glfw/include/GLFW/glfw3.h"
#include <stdbool.h>

typedef GLFWwindow TrioWindow;
typedef GLFWmonitor TrioMonitor;

#ifdef __cplusplus
extern "C" {
#endif

TrioWindow* TrioCreateWindow(const char * windowTitle, int windowWidth, int windowHeight, TrioMonitor* windowMonitor, TrioWindow* windowShare);
uint32_t TrioGetWindowCount();
void TrioSetCurrentWindow(TrioWindow* window);
bool TrioWindowShouldClose(TrioWindow* window);
void TrioCloseWindow(TrioWindow* window);

#ifdef __cplusplus
}
#endif
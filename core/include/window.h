#pragma once

#define RGFW_OPENGL

#include "../../external/rgfw/RGFW.h"
#include <stdbool.h>

typedef RGFW_window TrioWindow;

#ifdef __cplusplus
extern "C" {
#endif

TrioWindow* TrioCreateWindow(const char * windowTitle, int windowWidth, int windowHeight);
uint32_t TrioGetWindowCount();
void TrioSetCurrentWindow(TrioWindow* window);
bool TrioWindowShouldClose(TrioWindow* window);
void TrioCloseWindow(TrioWindow* window);

#ifdef __cplusplus
}
#endif
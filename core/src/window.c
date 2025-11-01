#include "../include/window.h"

bool init = false;
uint32_t windowCount = 0;

TrioWindow* TrioCreateWindow(const char * windowTitle, int windowWidth, int windowHeight, TrioMonitor* windowMonitor, TrioWindow* windowShare) {
    if (!init) {
        glfwInit();
        init = true;
    }

    TrioWindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, windowMonitor, windowShare);

    windowCount += 1;

    return window;
}

uint32_t TrioGetWindowCount() {
    return windowCount;
}

void TrioSetCurrentWindow(TrioWindow* window) {
    glfwMakeContextCurrent(window);
}

bool TrioWindowShouldClose(TrioWindow* window) {
    return glfwWindowShouldClose(window) == 1;
}

void TrioCloseWindow(TrioWindow* window) {
    glfwDestroyWindow(window);

    windowCount -= 1;
}
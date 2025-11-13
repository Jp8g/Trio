#include "../include/window.h"
#include "GLFW/glfw3.h"

bool glfwInitialised = false;
uint32_t windowCount = 0;

TrioWindow* TrioCreateWindow(const char * windowTitle, int windowWidth, int windowHeight, TrioMonitor* windowMonitor, TrioWindow* windowShare) {
    if (!glfwInitialised) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwInitialised = true;
    }

    TrioWindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, windowMonitor, windowShare);

    windowCount += 1;

    return window;
}

uint32_t TrioGetWindowCount(void) {
    return windowCount;
}

void TrioSetCurrentWindow(TrioWindow* window) {
    glfwMakeContextCurrent(window);
}

void TrioSetSwapInterval(int32_t interval) {
    glfwSwapInterval(interval);
}

bool TrioWindowShouldClose(TrioWindow* window) {
    return glfwWindowShouldClose(window) == 1;
}

void TrioCloseWindow(TrioWindow* window) {
    if (window) {
        glfwDestroyWindow(window);

        windowCount -= 1;
    }
}
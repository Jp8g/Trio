#define RGFW_IMPLEMENTATION

#include "../include/window.h"

bool glfwInitialised = false;
uint32_t windowCount = 0;

TrioWindow* TrioCreateWindow(const char * windowTitle, int windowWidth, int windowHeight) {


    RGFW_window* window = RGFW_createWindow(windowTitle, 0, 0, windowWidth, windowHeight, RGFW_windowCenter | RGFW_windowOpenGL);

    windowCount += 1;

    return window;
}

uint32_t TrioGetWindowCount(void) {
    return windowCount;
}

bool TrioWindowShouldClose(TrioWindow* window) {
    return RGFW_window_shouldClose(window) == RGFW_TRUE;
}

void TrioCloseWindow(TrioWindow* window) {
    if (window) {
        RGFW_window_close(window);

        windowCount -= 1;
    }
}
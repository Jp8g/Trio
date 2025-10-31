#include "../include/trio/window.h"

bool init = false;
int windowCount = 0;

TrioWindow* TrioCreateWindow(const char * Title, int Width, int Height) {
    if (!init) {
        glfwInit();
        init = true;
    }

    TrioWindow* window = glfwCreateWindow(Width, Height, Title, NULL, NULL);

    windowCount += 1;

    return window;
}

int TrioGetWindowCount() {
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
#include "../include/trio/graphics.h"

void TrioDisplayFrame(TrioWindow* window) {
    glfwSwapBuffers(window);
}
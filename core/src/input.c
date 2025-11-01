#include "../include/input.h"

void TrioPollEvents() {
    glfwPollEvents();
}

bool TrioIsKeyPressed(TrioWindow* window, TrioKey key) {
    return glfwGetKey(window, key) == 1;
}
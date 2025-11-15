#include "../include/input.h"

void TrioPollEvents() {
    RGFW_pollEvents();
}

bool TrioIsKeyPressed(TrioWindow* window, TrioKey key) {
    return RGFW_window_isKeyDown(window, key) == 1;
}
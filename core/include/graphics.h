#pragma once

#include "window.h"

typedef struct {
    float r;
    float g;
    float b;
    float a;
} TrioColor;

#ifdef __cplusplus
extern "C" {
#endif

void TrioDisplayFrame(TrioWindow* window);

#ifdef __cplusplus
}
#endif
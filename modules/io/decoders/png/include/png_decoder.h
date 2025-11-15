#pragma once

#include "../../../core/include/graphics.h"

#ifdef __cplusplus
extern "C" {
#endif

TrioImage* TrioLoadPng(const char* path, int32_t* width, int32_t* height);
void TrioFreePng(TrioImage* imageData);

#ifdef __cplusplus
}
#endif
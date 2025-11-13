#include "../include/png_decoder.h"
#include "graphics.h"

TrioImage* TrioLoadPng(const char* path, int32_t* width, int32_t* height) {
    int channels;
    char* resolvedPath = TrioResolvePath(path);
    TrioImage* imageData = stbi_load(resolvedPath, width, height, &channels, 3);
    free(resolvedPath);
    return imageData;
}

void TrioFreePng(TrioImage* imageData) {
    stbi_image_free(imageData);
}
#pragma once

#include <stdio.h>
#include "io.h"
#include "log.h"
#include "window.h"
#include "../../external/glad/include/glad/glad.h"

typedef struct TrioColor {
    float r;
    float g;
    float b;
    float a;
} TrioColor;

typedef struct TrioVec3{
    float x;
    float y;
    float z;
} TrioVec3;

typedef enum TrioShaderType {
    TRIO_SHADER_TYPE_VERTEX = GL_VERTEX_SHADER,
    TRIO_SHADER_TYPE_FRAGMENT = GL_FRAGMENT_SHADER,
} TrioShaderType;

#ifdef __cplusplus
extern "C" {
#endif

void TrioInitGraphics(TrioWindow* window);
uint32_t TrioCompileShader(const char* path, TrioShaderType shaderType);
void TrioDisplayFrame(TrioWindow* window);

#ifdef __cplusplus
}
#endif
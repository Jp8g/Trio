#pragma once

#include "io.h"
#include "window.h"
#include "../../external/glad/include/glad/glad.h"
#include "../stb_image.h"

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

typedef enum TrioVertexFormat {
    BYTE = 0x1400,
    UNSIGNED_BYTE = 0x1401,
    SHORT = 0x1402,
    UNSIGNED_SHORT = 0x1403,
    INT = 0x1404,
    UNSIGNED_INT = 0x1405,
    FLOAT = 0x1406,
} TrioVertexFormat;

typedef enum TrioBufferObjectUsage {
    USAGE_STREAM = 0x88E0,
    USAGE_STATIC = 0x88E4,
    USAGE_DYNAMIC = 0x88E8,
} TrioBufferObjectUsage;

typedef enum TrioDrawMode {
    POINTS,
    LINES,
    LINE_LOOP,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
} TrioDrawMode;

typedef unsigned char TrioImage;

#ifdef __cplusplus
extern "C" {
#endif

void TrioInitGraphics(TrioWindow* window);
uint32_t TrioCreateVAO();
uint32_t TrioCreateVBO(float* vertices, uint32_t size, TrioBufferObjectUsage drawMode);
uint32_t TrioCreateEBO(uint32_t* indices, uint32_t size, TrioBufferObjectUsage drawMode);
uint32_t TrioCreateTexture(uint32_t width, uint32_t height, TrioImage* imageData);
uint32_t TrioCompileShader(const char* path, TrioShaderType shaderType);
void TrioVertexAttributePointer(uint8_t index, uint32_t size, TrioVertexFormat vertexFormat, bool normalized, uint32_t stride, uintptr_t strideOffset);
uint32_t TrioCreateShaderProgram(uint32_t vertexShader, uint32_t fragmentShader);
void TrioClearColor(float red, float green, float blue, float alpha);
void TrioUseShaderProgram(uint32_t shaderProgram);
int32_t TrioGetUniformLocation(uint32_t shaderProgram, const char* uniformName);
void TrioSetUniform3f(uint32_t shaderProgram, int32_t location, float v0, float v1, float v2);
void TrioDrawElements(uint32_t shaderProgram, TrioDrawMode drawMode, uint32_t count, TrioVertexFormat vertexFormat, uint32_t offset);
void TrioDisplayFrame(TrioWindow* window);

#ifdef __cplusplus
}
#endif
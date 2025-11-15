#pragma once

#include "io.h"
#include "window.h"
#include "../../external/glad/include/glad/glad.h"
#include "../../external/stb_image.h"

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
    TRIO_BYTE = GL_BYTE,
    TRIO_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    TRIO_SHORT = GL_SHORT,
    TRIO_UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    TRIO_INT = GL_INT,
    TRIO_UNSIGNED_INT = GL_UNSIGNED_INT,
    TRIO_FLOAT = GL_FLOAT,
} TrioVertexFormat;

typedef enum TrioBufferObjectUsage {
    TRIO_USAGE_STREAM = GL_STREAM_DRAW,
    TRIO_USAGE_STATIC = GL_STATIC_DRAW,
    TRIO_USAGE_DYNAMIC = GL_DYNAMIC_DRAW,
} TrioBufferObjectUsage;

typedef enum TrioTextureType {
    TRIO_TEXTURE_1D = GL_TEXTURE_1D,
    TRIO_TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
    TRIO_TEXTURE_2D = GL_TEXTURE_2D,
    TRIO_TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
    TRIO_TEXTURE_3D = GL_TEXTURE_3D,
    TRIO_TEXTURE_CUBEMAP = GL_TEXTURE_CUBE_MAP,
    TRIO_TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
    TRIO_TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
} TrioTextureType;

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

void TrioSetUniform1f(uint32_t shaderProgram, int32_t location, float v0);
void TrioSetUniform2f(uint32_t shaderProgram, int32_t location, float v0, float v1);
void TrioSetUniform3f(uint32_t shaderProgram, int32_t location, float v0, float v1, float v2);
void TrioSetUniform4f(uint32_t shaderProgram, int32_t location, float v0, float v1, float v2, float v3);

void TrioSetUniform1i(uint32_t shaderProgram, int32_t location, int v0);
void TrioSetUniform2i(uint32_t shaderProgram, int32_t location, int v0, int v1);
void TrioSetUniform3i(uint32_t shaderProgram, int32_t location, int v0, int v1, int v2);
void TrioSetUniform4i(uint32_t shaderProgram, int32_t location, int v0, int v1, int v2, int v3);

void TrioSetActiveTexture(uint8_t textureIndex);
void TrioBindTexture(uint32_t shaderProgram, uint8_t textureIndex, TrioTextureType textureType, uint32_t texture);

void TrioDrawElements(uint32_t shaderProgram, TrioDrawMode drawMode, uint32_t count, TrioVertexFormat vertexFormat, uint32_t offset);
void TrioDisplayFrame(TrioWindow* window);

#ifdef __cplusplus
}
#endif
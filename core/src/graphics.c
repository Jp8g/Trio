#include "../include/graphics.h"

bool openGLInit = false;

void FramebufferSizeCallback(TrioWindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void TrioInitGraphics(TrioWindow* window) {
    if (!openGLInit) {
        gladLoadGL();
        openGLInit = true;
    }

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
}

uint32_t TrioCompileShader(const char* path, TrioShaderType shaderType) {

    char* resolvedPath = TrioResolvePath(path);
    FILE* shaderFile = fopen(resolvedPath, "r");
    if (!shaderFile) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to open file \"%s\"", resolvedPath);
        free(resolvedPath);
        return 0;
    }

    if (fseek(shaderFile, 0, SEEK_END) != 0) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to seek file \"%s\"", resolvedPath);
        fclose(shaderFile);
        free(resolvedPath);
        return 0;
    }

    long size = ftell(shaderFile);

    if (size < 0) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to get current position of file stream in file \"%s\"", resolvedPath);
        fclose(shaderFile);
        free(resolvedPath);
        return 0;
    }

    size_t fileSize = (size_t)size;

    if (fseek(shaderFile, 0, SEEK_SET) != 0) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to seek file \"%s\"", resolvedPath);
        fclose(shaderFile);
        free(resolvedPath);
        return 0;
    }

    char* shaderFileCharPtr = malloc(fileSize + 1);
    if (!shaderFileCharPtr) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes) for Shader file data (char* shaderFileCharPtr)", fileSize + 1);
        fclose(shaderFile);
        free(resolvedPath);
        return 0;
    }

    size_t readSize = fread(shaderFileCharPtr, 1, fileSize, shaderFile);
    if (readSize != fileSize) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to read file \"%s\"", resolvedPath);
        fclose(shaderFile);
        free(resolvedPath);
        free(shaderFileCharPtr);
        return 0;
    }

    free(resolvedPath);

    fclose(shaderFile);

    shaderFileCharPtr[fileSize] = '\0';

    uint32_t shader;
    shader = glCreateShader(shaderType);
    if (shader == 0) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to create shader");
        free(shaderFileCharPtr);
        return 0;
    }

    const char* const shaderSource = shaderFileCharPtr;

    glShaderSource(shader, 1, &shaderSource, NULL);
    free(shaderFileCharPtr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to compile shader: \"%s\"", infoLog);
        glad_glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void TrioDisplayFrame(TrioWindow* window) {
    glfwSwapBuffers(window);
}
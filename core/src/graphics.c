#define STB_IMAGE_IMPLEMENTATION

#include "../include/graphics.h"

bool openGLInit = false;
static uint32_t currentShaderProgram = 0;

void FramebufferSizeCallback(TrioWindow* window, int width, int height) {
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
}

void TrioInitGraphics(TrioWindow* window) {
    if (!openGLInit) {
        gladLoadGL();
        openGLInit = true;
    }

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
}

uint32_t TrioCreateVAO(void) {
    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    return VAO;
}

uint32_t TrioCreateVBO(float* vertices, uint32_t size, TrioBufferObjectUsage drawMode) {
    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, drawMode);

    return VBO;
}

uint32_t TrioCreateEBO(uint32_t* indices, uint32_t size, TrioBufferObjectUsage drawMode) {
    uint32_t EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, drawMode);

    return EBO;
}


uint32_t TrioCreateTexture(uint32_t width, uint32_t height, TrioImage* imageData) {
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
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

uint32_t TrioCreateShaderProgram(uint32_t vertexShader, uint32_t fragmentShader) {
    uint32_t shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glad_glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to link shader program: \"%s\"", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void TrioVertexAttributePointer(uint8_t index, uint32_t size, TrioVertexFormat vertexFormat, bool normalized, uint32_t stride, uintptr_t strideOffset) {
    glVertexAttribPointer(index, size, vertexFormat, normalized, stride, (void*)strideOffset);
    glEnableVertexAttribArray(index);
}

void TrioClearColor(float red, float green, float blue, float alpha) {
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

void TrioUseShaderProgram(uint32_t shaderProgram) {
    glUseProgram(shaderProgram);
    currentShaderProgram = shaderProgram;
}

int32_t TrioGetUniformLocation(uint32_t shaderProgram, const char* uniformName) {
    if (shaderProgram != currentShaderProgram) {
        TrioUseShaderProgram(shaderProgram);
    }

    return glGetUniformLocation(shaderProgram, uniformName);
}

void TrioSetUniform3f(uint32_t shaderProgram, int32_t location, float v0, float v1, float v2) {
    if (shaderProgram != currentShaderProgram) {
        TrioUseShaderProgram(shaderProgram);
    }

    glUniform3f(location, v0, v1, v2);
}

void TrioDrawElements(uint32_t shaderProgram, TrioDrawMode drawMode, uint32_t count, TrioVertexFormat vertexFormat, uint32_t offset) {
    if (shaderProgram != currentShaderProgram) {
        TrioUseShaderProgram(shaderProgram);
    }

    glDrawElements(drawMode, count, vertexFormat, (void*)(offset * sizeof(uint32_t)));
}

void TrioDisplayFrame(TrioWindow* window) {
    glfwSwapBuffers(window);
}
#include "football/render/Renderer.hpp"

#include <android/log.h>

namespace football {
namespace {
constexpr char kLogTag[] = "FootballLegendsRender";

constexpr char kVertexShader[] = R"(#version 300 es
layout(location = 0) in vec2 aPosition;
void main() {
    gl_Position = vec4(aPosition, 0.0, 1.0);
}
)";

constexpr char kFragmentShader[] = R"(#version 300 es
precision mediump float;
out vec4 outColor;
void main() {
    outColor = vec4(0.12, 0.78, 0.72, 1.0);
}
)";
}

Renderer::~Renderer() {
    destroy();
}

GLuint Renderer::compileShader(GLenum type, const char* source) {
    const GLuint shader = glCreateShader(type);
    if (shader == 0) {
        return 0;
    }

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 1) {
            char log[1024]{};
            glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
            __android_log_print(ANDROID_LOG_ERROR, kLogTag, "Shader compilation failed: %s", log);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

void Renderer::initialize() {
    destroy();

    const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, kVertexShader);
    const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, kFragmentShader);
    if (vertexShader == 0 || fragmentShader == 0) {
        if (vertexShader != 0) glDeleteShader(vertexShader);
        if (fragmentShader != 0) glDeleteShader(fragmentShader);
        return;
    }

    program_ = glCreateProgram();
    glAttachShader(program_, vertexShader);
    glAttachShader(program_, fragmentShader);
    glLinkProgram(program_);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint linked = GL_FALSE;
    glGetProgramiv(program_, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        GLint logLength = 0;
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 1) {
            char log[1024]{};
            glGetProgramInfoLog(program_, sizeof(log), nullptr, log);
            __android_log_print(ANDROID_LOG_ERROR, kLogTag, "Program link failed: %s", log);
        }
        destroy();
        return;
    }

    constexpr GLfloat vertices[] = {
        -0.48F, -0.42F,
         0.48F, -0.42F,
         0.00F,  0.48F
    };

    glGenVertexArrays(1, &vertexArray_);
    glGenBuffers(1, &vertexBuffer_);
    glBindVertexArray(vertexArray_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    initialized_ = vertexArray_ != 0 && vertexBuffer_ != 0;
    if (!initialized_) {
        __android_log_print(ANDROID_LOG_ERROR, kLogTag, "OpenGL buffers could not be created.");
        destroy();
        return;
    }

    __android_log_print(ANDROID_LOG_INFO, kLogTag, "OpenGL ES 3 renderer initialized.");
}

void Renderer::resize(int width, int height) {
    glViewport(0, 0, width > 0 ? width : 1, height > 0 ? height : 1);
}

void Renderer::render() {
    glClearColor(0.025F, 0.045F, 0.085F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!initialized_) {
        return;
    }

    glUseProgram(program_);
    glBindVertexArray(vertexArray_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Renderer::destroy() {
    if (vertexBuffer_ != 0) {
        glDeleteBuffers(1, &vertexBuffer_);
        vertexBuffer_ = 0;
    }
    if (vertexArray_ != 0) {
        glDeleteVertexArrays(1, &vertexArray_);
        vertexArray_ = 0;
    }
    if (program_ != 0) {
        glDeleteProgram(program_);
        program_ = 0;
    }
    initialized_ = false;
}

} // namespace football

#include "football/render/Renderer.hpp"

#include <GLES3/gl3.h>
#include <cmath>
#include <vector>

namespace football::render {
namespace {
struct Vertex {
    float x, y, z;
    float r, g, b;
};

GLuint compileShader(GLenum type, const char* source) {
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint createProgram() {
    constexpr char vertexSource[] = R"(#version 300 es
        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aColor;
        out vec3 vColor;
        void main() {
            gl_Position = vec4(aPosition, 1.0);
            vColor = aColor;
        }
    )";
    constexpr char fragmentSource[] = R"(#version 300 es
        precision mediump float;
        in vec3 vColor;
        out vec4 outColor;
        void main() {
            outColor = vec4(vColor, 1.0);
        }
    )";

    const GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexSource);
    const GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (vertex == 0 || fragment == 0) {
        if (vertex != 0) glDeleteShader(vertex);
        if (fragment != 0) glDeleteShader(fragment);
        return 0;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

void appendLine(std::vector<Vertex>& vertices,
                float x1, float y1, float x2, float y2) {
    constexpr float white = 0.92F;
    vertices.push_back({x1, y1, 0.0F, white, white, white});
    vertices.push_back({x2, y2, 0.0F, white, white, white});
}
} // namespace

void Renderer::onSurfaceCreated() {
    glClearColor(0.025F, 0.055F, 0.10F, 1.0F);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    program_ = createProgram();
    if (program_ == 0) return;

    glGenVertexArrays(1, &vertexArray_);
    glGenBuffers(1, &vertexBuffer_);
    glBindVertexArray(vertexArray_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glBindVertexArray(0);
}

void Renderer::onSurfaceChanged(int width, int height) {
    width_ = width > 0 ? width : 1;
    height_ = height > 0 ? height : 1;
    glViewport(0, 0, width_, height_);
}

void Renderer::drawFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (program_ == 0 || vertexArray_ == 0 || vertexBuffer_ == 0) return;

    // Perspective-style pitch blockout. Final camera, stadium meshes and lighting
    // will be introduced in the dedicated 3D rendering milestone.
    const std::vector<Vertex> field = {
        {-0.88F, -0.82F, 0.20F, 0.08F, 0.43F, 0.19F},
        { 0.88F, -0.82F, 0.20F, 0.08F, 0.43F, 0.19F},
        { 0.54F,  0.72F, 0.20F, 0.12F, 0.54F, 0.23F},
        {-0.88F, -0.82F, 0.20F, 0.08F, 0.43F, 0.19F},
        { 0.54F,  0.72F, 0.20F, 0.12F, 0.54F, 0.23F},
        {-0.54F,  0.72F, 0.20F, 0.12F, 0.54F, 0.23F}
    };

    std::vector<Vertex> lines;
    appendLine(lines, -0.88F, -0.82F, 0.88F, -0.82F);
    appendLine(lines, 0.88F, -0.82F, 0.54F, 0.72F);
    appendLine(lines, 0.54F, 0.72F, -0.54F, 0.72F);
    appendLine(lines, -0.54F, 0.72F, -0.88F, -0.82F);
    appendLine(lines, 0.0F, -0.82F, 0.0F, 0.72F);
    appendLine(lines, -0.71F, -0.08F, 0.71F, -0.08F);

    constexpr int circleSegments = 48;
    constexpr float centerY = -0.08F;
    constexpr float radiusX = 0.16F;
    constexpr float radiusY = 0.19F;
    for (int i = 0; i < circleSegments; ++i) {
        const float a = (2.0F * 3.14159265F * i) / circleSegments;
        const float b = (2.0F * 3.14159265F * (i + 1)) / circleSegments;
        appendLine(lines,
            std::cos(a) * radiusX, centerY + std::sin(a) * radiusY,
            std::cos(b) * radiusX, centerY + std::sin(b) * radiusY);
    }

    glUseProgram(program_);
    glBindVertexArray(vertexArray_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);

    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(field.size() * sizeof(Vertex)),
                 field.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(field.size()));

    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(lines.size() * sizeof(Vertex)),
                 lines.data(), GL_DYNAMIC_DRAW);
    glLineWidth(2.0F);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lines.size()));

    glBindVertexArray(0);
    glUseProgram(0);
}
} // namespace football::render

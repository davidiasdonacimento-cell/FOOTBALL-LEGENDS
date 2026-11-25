#pragma once

#include <GLES3/gl3.h>

namespace football {

class Renderer final {
public:
    Renderer() = default;
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void initialize();
    void resize(int width, int height);
    void render();

private:
    GLuint compileShader(GLenum type, const char* source);
    void destroy();

    GLuint program_{0};
    GLuint vertexArray_{0};
    GLuint vertexBuffer_{0};
    GLint colorLocation_{-1};
    bool initialized_{false};
};

} // namespace football

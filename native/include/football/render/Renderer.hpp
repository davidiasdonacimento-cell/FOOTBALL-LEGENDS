#pragma once

#include <GLES3/gl3.h>

#include "football/lobby/LobbyScene.hpp"

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
    void setFormation(int formation);
    void setCinematicCamera(bool enabled);

private:
    GLuint compileShader(GLenum type, const char* source);
    void destroy();
    void uploadScene();
    void createMaterialTexture();

    LobbyScene scene_;
    GLuint program_{0};
    GLuint environmentVao_{0};
    GLuint environmentBuffer_{0};
    GLuint playerVao_{0};
    GLuint playerBuffer_{0};
    GLint mvpLocation_{-1};
    GLint modelLocation_{-1};
    GLint cameraLocation_{-1};
    GLint lightLocation_{-1};
    GLint bonesLocation_{-1};
    GLint textureLocation_{-1};
    GLuint materialTexture_{0};
    int width_{1};
    int height_{1};
    bool initialized_{false};
    bool cinematicCamera_{true};
};

} // namespace football
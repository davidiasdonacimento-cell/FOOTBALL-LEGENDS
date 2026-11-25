#pragma once

namespace football::render {
class Renderer {
public:
    void onSurfaceCreated();
    void onSurfaceChanged(int width, int height);
    void drawFrame();

private:
    int width_{1};
    int height_{1};
    unsigned int program_{0};
    unsigned int vertexArray_{0};
    unsigned int vertexBuffer_{0};
};
}

#include "football/render/Renderer.hpp"
#include <GLES3/gl3.h>

namespace football::render {
void Renderer::onSurfaceCreated() {
    glClearColor(0.025F, 0.16F, 0.09F, 1.0F);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::onSurfaceChanged(int width, int height) {
    width_ = width > 0 ? width : 1;
    height_ = height > 0 ? height : 1;
    glViewport(0, 0, width_, height_);
}

void Renderer::drawFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
}

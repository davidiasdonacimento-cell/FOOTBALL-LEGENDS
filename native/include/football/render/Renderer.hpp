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
};
}

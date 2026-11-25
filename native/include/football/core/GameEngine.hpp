#pragma once

#include "football/render/Renderer.hpp"

namespace football {

class GameEngine final {
public:
    GameEngine() = default;
    ~GameEngine() = default;

    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    void onSurfaceCreated();
    void onSurfaceChanged(int width, int height);
    void renderFrame();

private:
    Renderer renderer_;
};

} // namespace football

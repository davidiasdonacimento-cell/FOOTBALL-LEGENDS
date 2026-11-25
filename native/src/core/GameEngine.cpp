#include "football/core/GameEngine.hpp"

namespace football {

void GameEngine::onSurfaceCreated() {
    renderer_.initialize();
}

void GameEngine::onSurfaceChanged(int width, int height) {
    renderer_.resize(width, height);
}

void GameEngine::renderFrame() {
    renderer_.render();
}

} // namespace football

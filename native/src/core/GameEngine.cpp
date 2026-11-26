#include "football/core/GameEngine.hpp"

namespace football {

void GameEngine::onSurfaceCreated() { renderer_.initialize(); }
void GameEngine::onSurfaceChanged(int width, int height) { renderer_.resize(width, height); }
void GameEngine::renderFrame() {
    match_.update(1.0f / 60.0f);
    renderer_.render();
}
void GameEngine::setLobbyFormation(int formation) { renderer_.setFormation(formation); }
void GameEngine::setCinematicCamera(bool enabled) { renderer_.setCinematicCamera(enabled); }
void GameEngine::startMatch() { match_.start(); }
void GameEngine::pauseMatch() { match_.pause(); }
void GameEngine::resumeMatch() { match_.resume(); }
void GameEngine::resetMatch() { match_.reset(); }

} // namespace football

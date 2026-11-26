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
void GameEngine::setControlRole(int role) {
    match_.setControlRole(role == 1 ? ControlRole::Referee : ControlRole::Player);
}
bool GameEngine::selectPlayer(int playerId) { return match_.selectControlledPlayer(playerId); }
bool GameEngine::movePlayer(float x, float y) { return match_.setPlayerMovement(x, y); }
bool GameEngine::playerAction(int action, int targetPlayerId, float x, float y) {
    if (action < 0 || action > 4) return false;
    return match_.command(match_.controlledPlayer(), static_cast<PlayerAction>(action), targetPlayerId, x, y);
}
bool GameEngine::refereeSendOff(int playerId) {
    if (match_.controlRole() != ControlRole::Referee) return false;
    return match_.setPlayerSentOff(playerId, true);
}

} // namespace football

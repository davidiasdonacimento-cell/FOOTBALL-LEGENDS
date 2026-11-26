#include "football/core/GameEngine.hpp"

namespace football {

void GameEngine::onSurfaceCreated() { renderer_.initialize(); }
void GameEngine::onSurfaceChanged(int width, int height) { renderer_.resize(width, height); }
void GameEngine::renderFrame() {
    const auto now = std::chrono::steady_clock::now();
    const float delta = hasPreviousFrame_
        ? std::chrono::duration<float>(now - previousFrame_).count()
        : (1.0f / 60.0f);
    previousFrame_ = now;
    hasPreviousFrame_ = true;
    match_.update(delta);
    renderer_.setMatchSnapshot(match_);
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
    if (action < 0 || action > 9) return false;
    return match_.command(match_.controlledPlayer(), static_cast<PlayerAction>(action), targetPlayerId, x, y);
}
bool GameEngine::refereeSendOff(int playerId) {
    if (match_.controlRole() != ControlRole::Referee) return false;
    return match_.setPlayerSentOff(playerId, true);
}
bool GameEngine::refereeIssueCard(int playerId, int cardType) { return match_.issueCard(playerId, cardType); }
bool GameEngine::refereeCallFoul(int playerId) { return match_.callFoul(playerId); }
bool GameEngine::refereePlayAdvantage(int playerId) { return match_.playAdvantage(playerId); }
bool GameEngine::refereeCallOffside(int playerId) { return match_.callOffside(playerId); }
bool GameEngine::refereeRequestVar() { return match_.requestVarReview(); }
bool GameEngine::refereeResolveVar(bool uphold) { return match_.resolveVar(uphold); }
bool GameEngine::refereeRestartPlay() { return match_.restartPlay(); }
void GameEngine::setMatchTimeScale(float scale) { match_.setTimeScale(scale); }
bool GameEngine::setAIDifficulty(int difficulty) {
    if (difficulty < 0 || difficulty > 2) return false;
    return match_.setAIDifficulty(static_cast<AIDifficulty>(difficulty));
}

} // namespace football

#pragma once

#include "football/core/MatchSimulation.hpp"
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
    void setLobbyFormation(int formation);
    void setCinematicCamera(bool enabled);
    void startMatch();
    void pauseMatch();
    void resumeMatch();
    void resetMatch();
    void setControlRole(int role);
    bool selectPlayer(int playerId);
    bool movePlayer(float x, float y);
    bool playerAction(int action, int targetPlayerId, float x, float y);
    bool refereeSendOff(int playerId);
    bool refereeIssueCard(int playerId, int cardType);
    const MatchSimulation& match() const { return match_; }

private:
    Renderer renderer_;
    MatchSimulation match_;
};

} // namespace football

#pragma once
#include <array>
#include <cstdint>

namespace football {

struct Vec2 { float x{0.0f}; float y{0.0f}; };
enum class MatchState : std::uint8_t { Ready, Running, Paused, Finished };
enum class PlayerAction : std::uint8_t { Pass, Shoot, Dribble, Sprint, Tackle };
enum class ControlRole : std::uint8_t { Player, Referee };

struct Player {
    int id{0};
    int team{0};
    int role{0};
    Vec2 position{};
    Vec2 velocity{};
    float stamina{1.0f};
    bool sentOff{false};
};

struct Ball {
    Vec2 position{};
    Vec2 velocity{};
    int owner{-1};
    int lastTouchTeam{-1};
};

class MatchSimulation final {
public:
    static constexpr int kPlayersPerTeam = 11;
    static constexpr int kPlayerCount = 22;
    static constexpr float kPitchHalfLength = 55.0f;
    static constexpr float kPitchHalfWidth = 36.0f;

    MatchSimulation();
    void start();
    void pause();
    void resume();
    void finish();
    void reset();
    void update(float deltaSeconds);

    bool command(int playerId, PlayerAction action, int targetPlayerId = -1,
                 float directionX = 0.0f, float directionY = 0.0f);
    bool setPlayerSentOff(int playerId, bool sentOff);
    void setSpectatorMode(bool enabled);
    void setControlRole(ControlRole role);
    ControlRole controlRole() const { return controlRole_; }
    bool selectControlledPlayer(int playerId);
    bool setPlayerMovement(float x, float y);
    int controlledPlayer() const { return controlledPlayer_; }

    MatchState state() const { return state_; }
    float matchSeconds() const { return matchSeconds_; }
    int playerCount() const { return kPlayerCount; }
    int activePlayerCount(int team) const;
    int score(int team) const;
    const std::array<Player, kPlayerCount>& players() const { return players_; }
    const Ball& ball() const { return ball_; }
    bool spectatorMode() const { return spectatorMode_; }

private:
    void initializeTeams();
    void updateTeam(float dt, int team);
    void updateBall(float dt);
    void clampPlayer(Player& player);
    int nearestPlayer(int team, Vec2 point) const;
    static Vec2 normalized(Vec2 value);
    static float distance(Vec2 a, Vec2 b);

    std::array<Player, kPlayerCount> players_{};
    Ball ball_{};
    MatchState state_{MatchState::Ready};
    float matchSeconds_{0.0f};
    int scores_[2]{0, 0};
    bool spectatorMode_{false};
    ControlRole controlRole_{ControlRole::Player};
    int controlledPlayer_{10};
    Vec2 playerInput_{};
};

} // namespace football

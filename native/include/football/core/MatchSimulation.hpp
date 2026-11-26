#pragma once
#include <array>
#include <cstdint>

namespace football {

struct Vec2 { float x{0.0f}; float y{0.0f}; };
enum class MatchState : std::uint8_t { Ready, Running, Paused, Finished };
enum class PlayerAction : std::uint8_t { Pass, Shoot, Dribble, Sprint, Tackle };
enum class ControlRole : std::uint8_t { Player, Referee };
enum class MatchEvent : std::uint8_t { None, Foul, Offside, Penalty, VarReview, Goal };

struct Player {
    int id{0};
    int team{0};
    int role{0};
    Vec2 position{};
    Vec2 velocity{};
    float stamina{1.0f};
    bool sentOff{false};
    int yellowCards{0};
    float protestSeconds{0.0f};
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
    static constexpr float kPitchHalfLength = 36.0f;
    static constexpr float kPitchHalfWidth = 55.0f;

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
    bool issueCard(int playerId, int cardType);
    bool callFoul(int playerId);
    bool callOffside(int playerId);
    bool requestVarReview();
    bool resolveVar(bool upholdDecision);
    bool restartPlay();
    void setTimeScale(float scale);
    float timeScale() const { return timeScale_; }
    MatchEvent event() const { return event_; }
    int eventPlayer() const { return eventPlayer_; }
    int restartTeam() const { return restartTeam_; }
    Vec2 refereePosition() const { return refereePosition_; }
    float offsideLine() const { return offsideLine_; }
    bool offsideLineVisible() const { return offsideLineVisible_; }
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
    void updateReferee(float dt);
    void registerFoul(int playerId);
    bool detectOffside(int attackingTeam, int receiverId);
    void clearEvent();
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
    float playerSpeedMultiplier_{1.0f};
    MatchEvent event_{MatchEvent::None};
    MatchEvent eventBeforeVar_{MatchEvent::None};
    Ball ballBeforeVar_{};
    int eventPlayer_{-1};
    int restartTeam_{-1};
    Vec2 restartPosition_{};
    Vec2 refereePosition_{};
    float offsideLine_{0.0f};
    bool offsideLineVisible_{false};
    float timeScale_{1.0f};
    float cinematicSlowMoRemaining_{0.0f};
    float previousTimeScale_{1.0f};
};

} // namespace football

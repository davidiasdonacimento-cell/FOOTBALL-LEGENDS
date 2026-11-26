#include "football/core/MatchSimulation.hpp"
#include <algorithm>
#include <cmath>

namespace football {
namespace {
constexpr float kPlayerSpeed = 6.2f;
constexpr float kBallFriction = 0.985f;
constexpr float kControlRadius = 1.65f;
constexpr Vec2 kFormation[11] = {
    {-48.0f,0.0f},{-37.0f,-22.0f},{-39.0f,-10.0f},{-39.0f,10.0f},{-37.0f,22.0f},
    {-22.0f,-17.0f},{-24.0f,0.0f},{-22.0f,17.0f},
    {-8.0f,-20.0f},{-8.0f,20.0f},{-2.0f,0.0f}
};
}

MatchSimulation::MatchSimulation() { initializeTeams(); }

void MatchSimulation::initializeTeams() {
    for (int team = 0; team < 2; ++team) {
        for (int i = 0; i < kPlayersPerTeam; ++i) {
            Player& p = players_[team * kPlayersPerTeam + i];
            p.id = team * kPlayersPerTeam + i;
            p.team = team;
            p.role = i;
            const float sign = team == 0 ? 1.0f : -1.0f;
            p.position = {kFormation[i].x * sign, kFormation[i].y};
            p.velocity = {};
            p.stamina = 1.0f;
            p.sentOff = false;
        }
    }
    ball_ = {};
    ball_.position = {};
    ball_.owner = -1;
    ball_.lastTouchTeam = -1;
}

void MatchSimulation::reset() {
    state_ = MatchState::Ready;
    matchSeconds_ = 0.0f;
    scores_[0] = scores_[1] = 0;
    spectatorMode_ = false;
    initializeTeams();
}
void MatchSimulation::start() {
    if (state_ == MatchState::Ready) {
        state_ = MatchState::Running;
        ball_.owner = 10;
        ball_.lastTouchTeam = 0;
        ball_.position = players_[10].position;
    }
}
void MatchSimulation::pause() { if (state_ == MatchState::Running) state_ = MatchState::Paused; }
void MatchSimulation::resume() { if (state_ == MatchState::Paused) state_ = MatchState::Running; }
void MatchSimulation::finish() { state_ = MatchState::Finished; }
void MatchSimulation::setSpectatorMode(bool enabled) { spectatorMode_ = enabled; }

Vec2 MatchSimulation::normalized(Vec2 v) {
    const float length = std::sqrt(v.x * v.x + v.y * v.y);
    return length > 0.0001f ? Vec2{v.x / length, v.y / length} : Vec2{};
}
float MatchSimulation::distance(Vec2 a, Vec2 b) {
    const float x = a.x - b.x, y = a.y - b.y;
    return std::sqrt(x * x + y * y);
}
int MatchSimulation::nearestPlayer(int team, Vec2 point) const {
    int result = -1;
    float best = 1.0e9f;
    for (int i = team * 11; i < team * 11 + 11; ++i) {
        const Player& p = players_[i];
        if (p.sentOff) continue;
        const float d = distance(p.position, point);
        if (d < best) { best = d; result = i; }
    }
    return result;
}
int MatchSimulation::activePlayerCount(int team) const {
    if (team < 0 || team > 1) return 0;
    int count = 0;
    for (int i = team * 11; i < team * 11 + 11; ++i)
        if (!players_[i].sentOff) ++count;
    return count;
}
int MatchSimulation::score(int team) const {
    return team >= 0 && team < 2 ? scores_[team] : 0;
}
void MatchSimulation::clampPlayer(Player& p) {
    p.position.x = std::max(-kPitchHalfLength + 1.0f, std::min(kPitchHalfLength - 1.0f, p.position.x));
    p.position.y = std::max(-kPitchHalfWidth + 1.0f, std::min(kPitchHalfWidth - 1.0f, p.position.y));
}
void MatchSimulation::updateTeam(float dt, int team) {
    const int carrier = ball_.owner;
    const int carrierTeam = carrier >= 0 ? players_[carrier].team : -1;
    const int chaser = nearestPlayer(team, ball_.position);
    for (int i = team * 11; i < team * 11 + 11; ++i) {
        Player& p = players_[i];
        if (p.sentOff) { p.velocity = {}; continue; }
        Vec2 target = p.position;
        if (i == carrier && carrierTeam == team) {
            target = {team == 0 ? 48.0f : -48.0f, 0.0f};
        } else if (carrierTeam != team && i == chaser) {
            target = ball_.position;
        } else if (carrierTeam == team) {
            target = {p.position.x + (team == 0 ? 3.0f : -3.0f), p.position.y};
        } else {
            const float sign = team == 0 ? 1.0f : -1.0f;
            target = {kFormation[p.role].x * sign * 0.72f, kFormation[p.role].y * 0.85f};
        }
        Vec2 delta{target.x - p.position.x, target.y - p.position.y};
        Vec2 dir = normalized(delta);
        const float speed = kPlayerSpeed * (p.stamina < 0.15f ? 0.72f : 1.0f);
        p.velocity = {dir.x * speed, dir.y * speed};
        p.position.x += p.velocity.x * dt;
        p.position.y += p.velocity.y * dt;
        clampPlayer(p);
        p.stamina = std::max(0.0f, p.stamina - dt * 0.0008f);
    }
}
void MatchSimulation::updateBall(float dt) {
    if (ball_.owner >= 0) {
        const Player& owner = players_[ball_.owner];
        ball_.position = {owner.position.x + (owner.team == 0 ? 0.9f : -0.9f), owner.position.y};
        ball_.velocity = owner.velocity;
        if (distance(owner.position, ball_.position) > 3.0f) ball_.owner = -1;
        return;
    }
    ball_.position.x += ball_.velocity.x * dt;
    ball_.position.y += ball_.velocity.y * dt;
    const float drag = std::pow(kBallFriction, dt * 60.0f);
    ball_.velocity.x *= drag;
    ball_.velocity.y *= drag;
    if (std::fabs(ball_.position.y) > kPitchHalfWidth) {
        ball_.position.y = std::copysign(kPitchHalfWidth, ball_.position.y);
        ball_.velocity.y *= -0.55f;
    }
    if (std::fabs(ball_.position.x) > kPitchHalfLength) {
        const int scoringTeam = ball_.position.x > 0.0f ? 0 : 1;
        ++scores_[scoringTeam];
        ball_.position = {};
        ball_.velocity = {};
        ball_.owner = scoringTeam * 11 + 10;
        ball_.lastTouchTeam = scoringTeam;
    } else {
        for (int team = 0; team < 2; ++team) {
            const int candidate = nearestPlayer(team, ball_.position);
            if (candidate >= 0 && distance(players_[candidate].position, ball_.position) < kControlRadius) {
                ball_.owner = candidate;
                ball_.lastTouchTeam = team;
                break;
            }
        }
    }
}
void MatchSimulation::update(float deltaSeconds) {
    if (state_ != MatchState::Running || !std::isfinite(deltaSeconds) || deltaSeconds <= 0.0f) return;
    const float dt = std::min(deltaSeconds, 0.05f);
    matchSeconds_ += dt;
    updateTeam(dt, 0);
    updateTeam(dt, 1);
    updateBall(dt);
}
bool MatchSimulation::command(int playerId, PlayerAction action, int targetPlayerId,
                              float directionX, float directionY) {
    if (state_ != MatchState::Running || playerId < 0 || playerId >= kPlayerCount) return false;
    Player& player = players_[playerId];
    if (player.sentOff || (spectatorMode_ && player.team == 0)) return false;
    const Vec2 direction = normalized({directionX, directionY});
    switch (action) {
        case PlayerAction::Pass:
            if (ball_.owner != playerId || targetPlayerId < 0 || targetPlayerId >= kPlayerCount ||
                players_[targetPlayerId].team != player.team || players_[targetPlayerId].sentOff) return false;
            ball_.owner = -1;
            ball_.lastTouchTeam = player.team;
            ball_.position = player.position;
            ball_.velocity = {direction.x * 17.0f, direction.y * 17.0f};
            return true;
        case PlayerAction::Shoot:
            if (ball_.owner != playerId) return false;
            ball_.owner = -1;
            ball_.lastTouchTeam = player.team;
            ball_.position = player.position;
            ball_.velocity = {direction.x * 25.0f, direction.y * 25.0f};
            return true;
        case PlayerAction::Dribble:
        case PlayerAction::Sprint:
            if (action == PlayerAction::Sprint && player.stamina <= 0.0f) return false;
            player.velocity = {direction.x * (action == PlayerAction::Sprint ? 9.0f : 6.5f),
                               direction.y * (action == PlayerAction::Sprint ? 9.0f : 6.5f)};
            return true;
        case PlayerAction::Tackle: {
            const int opponent = nearestPlayer(1 - player.team, player.position);
            if (opponent < 0 || distance(player.position, players_[opponent].position) > 2.2f) return false;
            if (ball_.owner == opponent) {
                ball_.owner = playerId;
                ball_.lastTouchTeam = player.team;
                return true;
            }
            return false;
        }
    }
    return false;
}
bool MatchSimulation::setPlayerSentOff(int playerId, bool sentOff) {
    if (playerId < 0 || playerId >= kPlayerCount) return false;
    players_[playerId].sentOff = sentOff;
    if (sentOff && ball_.owner == playerId) ball_.owner = -1;
    return true;
}

} // namespace football

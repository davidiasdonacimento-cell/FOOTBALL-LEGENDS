#include "football/core/MatchSimulation.hpp"
#include "football/ai/TeamAI.hpp"
#include <algorithm>
#include <cmath>
#include <functional>

namespace football {
namespace {
constexpr float kPlayerSpeed = 6.2f;
constexpr float kBallFriction = 0.985f;
constexpr float kControlRadius = 1.65f;
constexpr Vec2 kFormation[11] = {
    {-32.0f,0.0f},{-24.0f,-35.0f},{-25.0f,-12.0f},{-25.0f,12.0f},{-24.0f,35.0f},
    {-14.0f,-20.0f},{-15.0f,0.0f},{-14.0f,20.0f},
    {-5.0f,-30.0f},{-5.0f,30.0f},{-2.0f,0.0f}
};
}

MatchSimulation::MatchSimulation() {
    initializeTeams();
    refereePosition_ = {0.0f, -5.0f};
}

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
            p.yellowCards = 0;
            p.protestSeconds = 0.0f;
            p.animation = PlayerAnimation::None;
            p.animationSeconds = 0.0f;
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
    tacticalStyles_[0] = tacticalStyles_[1] = TacticalStyle::Balanced;
    aiDecisionCooldown_.fill(0.0f);
    aiCarryIntent_.fill(false);
    controlRole_ = ControlRole::Player;
    controlledPlayer_ = 10;
    playerInput_ = {};
    playerSpeedMultiplier_ = 1.0f;
    initializeTeams();
    event_ = MatchEvent::None;
    eventBeforeVar_ = MatchEvent::None;
    ballBeforeVar_ = {};
    ballBeforeGoal_ = {};
    scoresBeforeGoal_[0] = scoresBeforeGoal_[1] = 0;
    scoresBeforeVar_[0] = scoresBeforeVar_[1] = 0;
    eventPlayer_ = -1;
    advantageActive_ = false;
    advantageTeam_ = -1;
    advantagePlayer_ = -1;
    advantageSeconds_ = 0.0f;
    advantageScoreBefore_[0] = advantageScoreBefore_[1] = 0;
    restartTeam_ = -1;
    restartPosition_ = {};
    refereePosition_ = {0.0f, -5.0f};
    offsideLine_ = 0.0f;
    offsideLineVisible_ = false;
    timeScale_ = 1.0f;
    cinematicSlowMoRemaining_ = 0.0f;
    previousTimeScale_ = 1.0f;
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
void MatchSimulation::resume() {
    if (state_ != MatchState::Paused || event_ == MatchEvent::VarReview) return;
    // A stoppage must use its designated restart, not simply unfreeze the clock.
    if (event_ != MatchEvent::None && restartTeam_ >= 0) {
        restartPlay();
        return;
    }
    state_ = MatchState::Running;
}
void MatchSimulation::finish() { state_ = MatchState::Finished; }
void MatchSimulation::setSpectatorMode(bool enabled) { spectatorMode_ = enabled; }
bool MatchSimulation::setTacticalStyle(int team, TacticalStyle style) {
    if (team < 0 || team > 1 || static_cast<unsigned int>(style) > static_cast<unsigned int>(TacticalStyle::DefensiveBlock)) return false;
    tacticalStyles_[team] = style;
    return true;
}
TacticalStyle MatchSimulation::tacticalStyle(int team) const {
    return team >= 0 && team < 2 ? tacticalStyles_[team] : TacticalStyle::Balanced;
}
void MatchSimulation::setControlRole(ControlRole role) {
    controlRole_ = role;
    playerInput_ = {};
    playerSpeedMultiplier_ = 1.0f;
}
bool MatchSimulation::selectControlledPlayer(int playerId) {
    if (playerId < 0 || playerId >= kPlayerCount || players_[playerId].team != 0 || players_[playerId].sentOff) return false;
    controlledPlayer_ = playerId;
    playerInput_ = {};
    return true;
}
bool MatchSimulation::setPlayerMovement(float x, float y) {
    if (state_ != MatchState::Running || controlRole_ != ControlRole::Player || !std::isfinite(x) || !std::isfinite(y)) return false;
    playerInput_ = normalized({std::max(-1.0f, std::min(1.0f, x)), std::max(-1.0f, std::min(1.0f, y))});
    if (playerInput_.x == 0.0f && playerInput_.y == 0.0f) playerSpeedMultiplier_ = 1.0f;
    return true;
}

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
        if (p.protestSeconds > 0.0f) p.protestSeconds = std::max(0.0f, p.protestSeconds - dt);
        if (p.animationSeconds > 0.0f) {
            p.animationSeconds = std::max(0.0f, p.animationSeconds - dt);
            if (p.animationSeconds == 0.0f && !p.sentOff) {
                if (p.role == 0 && p.animation == PlayerAnimation::KeeperDive) {
                    p.animation = PlayerAnimation::KeeperFall;
                    p.animationSeconds = 0.32f;
                } else if (p.role == 0 && p.animation == PlayerAnimation::KeeperFall) {
                    p.animation = PlayerAnimation::KeeperRecover;
                    p.animationSeconds = 0.55f;
                } else p.animation = PlayerAnimation::None;
            }
        }
        if (p.role == 0 && p.animation == PlayerAnimation::KeeperDive) {
            p.position.x += p.velocity.x * dt;
            p.position.y += p.velocity.y * dt;
            p.position.x = std::max(-35.0f, std::min(-26.0f, p.position.x));
            p.position.y = std::max(-15.0f, std::min(15.0f, p.position.y));
            if (ball_.owner < 0 && ball_.height <= 2.8f && distance(p.position, ball_.position) <= 2.8f) {
                ball_.owner = p.id; ball_.lastTouchTeam = p.team; ball_.velocity = {};
                ball_.height = 0.0f; ball_.verticalVelocity = 0.0f;
                p.animation = PlayerAnimation::KeeperCatch; p.animationSeconds = 0.7f;
            }
            continue;
        }
        if (p.role == 0 && (p.animation == PlayerAnimation::KeeperFall || p.animation == PlayerAnimation::KeeperRecover)) {
            p.velocity = {};
            continue;
        }
        if (p.sentOff) {
            p.velocity = {};
            if (p.protestSeconds <= 0.0f) p.position.y += (p.position.y >= 0.0f ? 1.0f : -1.0f) * 2.8f * dt;
            continue;
        }
        if (team == 0 && i == controlledPlayer_ && controlRole_ == ControlRole::Player) {
            p.velocity = {playerInput_.x * kPlayerSpeed * playerSpeedMultiplier_, playerInput_.y * kPlayerSpeed * playerSpeedMultiplier_};
            p.position.x += p.velocity.x * dt;
            p.position.y += p.velocity.y * dt;
            if (p.role == 0) {
                p.position.x = std::max(-35.0f, std::min(-26.0f, p.position.x));
                p.position.y = std::max(-15.0f, std::min(15.0f, p.position.y));
            } else clampPlayer(p);
            p.stamina = std::max(0.0f, p.stamina - dt * (playerInput_.x != 0.0f || playerInput_.y != 0.0f ? 0.0015f : 0.0003f));
            continue;
        }
        if (ball_.owner == i && aiCarryIntent_[i]) {
            const float attackDirection = team == 0 ? 1.0f : -1.0f;
            const Vec2 carryDirection = normalized({attackDirection, -p.position.y * 0.025f});
            p.velocity = {carryDirection.x * 6.6f, carryDirection.y * 6.6f};
            p.position.x += p.velocity.x * dt;
            p.position.y += p.velocity.y * dt;
            clampPlayer(p);
            p.stamina = std::max(0.0f, p.stamina - dt * 0.0010f);
            continue;
        }
        const TeamAIContext aiContext{team, i, p.role, p.position, ball_.position,
                                      carrierTeam, chaser, p.stamina, tacticalStyles_[team]};
        const Vec2 target = TeamAI::targetForPlayer(aiContext);
        Vec2 delta{target.x - p.position.x, target.y - p.position.y};
        Vec2 dir = normalized(delta);
        const float speed = kPlayerSpeed * TeamAI::movementSpeedMultiplier(aiContext) * (p.stamina < 0.15f ? 0.72f : 1.0f);
        p.velocity = {dir.x * speed, dir.y * speed};
        p.position.x += p.velocity.x * dt;
        p.position.y += p.velocity.y * dt;
        clampPlayer(p);
        p.stamina = std::max(0.0f, p.stamina - dt * 0.0008f);
    }
}

void MatchSimulation::updateAutonomousDecision(float dt) {
    for (int i = 0; i < kPlayerCount; ++i)
        aiDecisionCooldown_[i] = std::max(0.0f, aiDecisionCooldown_[i] - dt);
    if (ball_.owner < 0 || ball_.owner >= kPlayerCount) return;
    const int carrierId = ball_.owner;
    Player& carrier = players_[carrierId];
    if (carrier.sentOff || aiDecisionCooldown_[carrierId] > 0.0f) return;
    const bool humanOwnsCarrier = controlRole_ == ControlRole::Player &&
        !spectatorMode_ && carrier.team == 0 && carrierId == controlledPlayer_;
    if (humanOwnsCarrier) return;

    const int team = carrier.team;
    const int opponentStart = (1 - team) * kPlayersPerTeam;
    float nearestOpponent = 1000.0f;
    bool laneBlocked = false;
    const float attackDirection = team == 0 ? 1.0f : -1.0f;
    const Vec2 goal{36.0f * attackDirection, 0.0f};
    const Vec2 toGoal{goal.x - carrier.position.x, goal.y - carrier.position.y};
    const float goalLengthSquared = toGoal.x * toGoal.x + toGoal.y * toGoal.y;
    for (int i = opponentStart; i < opponentStart + kPlayersPerTeam; ++i) {
        const Player& opponent = players_[i];
        if (opponent.sentOff) continue;
        nearestOpponent = std::min(nearestOpponent, distance(carrier.position, opponent.position));
        if (goalLengthSquared > 0.001f) {
            const Vec2 relative{opponent.position.x - carrier.position.x,
                                opponent.position.y - carrier.position.y};
            const float projection = (relative.x * toGoal.x + relative.y * toGoal.y) /
                                     goalLengthSquared;
            if (projection > 0.12f && projection < 0.88f) {
                const Vec2 closest{carrier.position.x + toGoal.x * projection,
                                   carrier.position.y + toGoal.y * projection};
                if (distance(closest, opponent.position) < 2.8f) laneBlocked = true;
            }
        }
    }
    int bestTeammate = -1;
    float bestScore = -1.0e9f, bestProgress = 0.0f, bestPressure = 0.0f;
    for (int i = team * kPlayersPerTeam; i < (team + 1) * kPlayersPerTeam; ++i) {
        if (i == carrierId || players_[i].sentOff) continue;
        const Player& teammate = players_[i];
        const float progress = (teammate.position.x - carrier.position.x) * attackDirection;
        const float teammateDistance = distance(carrier.position, teammate.position);
        if (progress < 1.5f || teammateDistance > 30.0f) continue;
        float pressure = 1000.0f;
        for (int j = opponentStart; j < opponentStart + kPlayersPerTeam; ++j) {
            if (!players_[j].sentOff)
                pressure = std::min(pressure, distance(teammate.position, players_[j].position));
        }
        const float score = progress * 0.75f + std::min(pressure, 10.0f) * 0.55f -
                            teammateDistance * 0.12f;
        if (score > bestScore) {
            bestScore = score; bestTeammate = i;
            bestProgress = progress; bestPressure = pressure;
        }
    }
    const AIBallDecisionContext context{team, carrier.role, carrier.position,
        bestTeammate, bestProgress, bestPressure, nearestOpponent, laneBlocked,
        tacticalStyles_[team]};
    const AIBallAction decision = TeamAI::decideBallAction(context);
    aiDecisionCooldown_[carrierId] = 0.62f;
    if (decision == AIBallAction::Shoot) {
        aiCarryIntent_[carrierId] = false;
        executeCommand(carrierId, PlayerAction::Shoot, -1, toGoal.x, toGoal.y, true);
    } else if (decision == AIBallAction::Pass && bestTeammate >= 0) {
        aiCarryIntent_[carrierId] = false;
        const Vec2 passDirection{players_[bestTeammate].position.x - carrier.position.x,
                                 players_[bestTeammate].position.y - carrier.position.y};
        executeCommand(carrierId, PlayerAction::Pass, bestTeammate,
                       passDirection.x, passDirection.y, true);
    } else {
        aiCarryIntent_[carrierId] = true;
    }
}

void MatchSimulation::updateBall(float dt) {
    if (ball_.owner >= 0) {
        const Player& owner = players_[ball_.owner];
        ball_.position = {owner.position.x + (owner.team == 0 ? 0.9f : -0.9f), owner.position.y};
        ball_.velocity = owner.velocity;
        ball_.height = 0.0f; ball_.verticalVelocity = 0.0f;
        if (distance(owner.position, ball_.position) > 3.0f) ball_.owner = -1;
        return;
    }
    ball_.position.x += ball_.velocity.x * dt;
    ball_.position.y += ball_.velocity.y * dt;
    if (ball_.height > 0.0f || ball_.verticalVelocity > 0.0f) {
        ball_.height = std::max(0.0f, ball_.height + ball_.verticalVelocity * dt - 4.9f * dt * dt);
        ball_.verticalVelocity -= 9.8f * dt;
        if (ball_.height == 0.0f) {
            ball_.verticalVelocity = std::max(0.0f, -ball_.verticalVelocity * 0.28f);
            if (ball_.verticalVelocity < 0.65f) ball_.verticalVelocity = 0.0f;
        }
    }
    const float drag = std::pow(kBallFriction, dt * 60.0f);
    ball_.velocity.x *= drag;
    ball_.velocity.y *= drag;

    // Goal frame collision: two posts and a crossbar on each end line.
    constexpr float kGoalHalfWidth = 3.66f;
    constexpr float kGoalHeight = 2.44f;
    constexpr float kBallRadius = 0.11f;
    constexpr float kPostCollisionRadius = 0.28f;
    for (float goalX : {-kPitchHalfLength, kPitchHalfLength}) {
        for (float postY : {-kGoalHalfWidth, kGoalHalfWidth}) {
            const float dx = ball_.position.x - goalX;
            const float dy = ball_.position.y - postY;
            const float distanceSquared = dx * dx + dy * dy;
            if (distanceSquared < kPostCollisionRadius * kPostCollisionRadius &&
                ball_.height < kGoalHeight) {
                float nx = dx;
                float ny = dy;
                const float length = std::sqrt(nx * nx + ny * ny);
                if (length > 0.0001f) { nx /= length; ny /= length; }
                else { nx = goalX > 0.0f ? 1.0f : -1.0f; ny = 0.0f; }
                const float normalSpeed = ball_.velocity.x * nx + ball_.velocity.y * ny;
                ball_.velocity.x -= 1.55f * normalSpeed * nx;
                ball_.velocity.y -= 1.55f * normalSpeed * ny;
                ball_.position.x = goalX - std::copysign(kPostCollisionRadius + 0.02f, goalX);
                ball_.position.y = postY + ny * (kPostCollisionRadius + 0.02f);
            }
        }
        if (std::fabs(ball_.position.x - goalX) < 0.30f &&
            std::fabs(ball_.position.y) < kGoalHalfWidth &&
            ball_.height >= kGoalHeight - 0.24f &&
            ball_.height <= kGoalHeight + 0.24f) {
            ball_.height = kGoalHeight - 0.30f;
            ball_.verticalVelocity = -std::max(1.2f, std::fabs(ball_.verticalVelocity) * 0.55f);
            ball_.velocity.x *= -0.35f;
        }
    }

    if (std::fabs(ball_.position.x) > kPitchHalfLength) {
        const float goalLine = std::copysign(kPitchHalfLength, ball_.position.x);
        const int scoringTeam = ball_.position.x > 0.0f ? 0 : 1;
        const bool insideGoal = std::fabs(ball_.position.y) <= kGoalHalfWidth - kBallRadius &&
                                ball_.height <= kGoalHeight - kBallRadius;
        if (insideGoal) {
            ballBeforeGoal_ = ball_;
            scoresBeforeGoal_[0] = scores_[0];
            scoresBeforeGoal_[1] = scores_[1];
            ++scores_[scoringTeam];
            event_ = MatchEvent::Goal;
            eventPlayer_ = -1;
            restartTeam_ = 1 - scoringTeam;
            restartPosition_ = {};
            ball_.position = {};
            ball_.velocity = {};
            ball_.height = 0.0f;
            ball_.verticalVelocity = 0.0f;
            ball_.owner = -1;
            ball_.lastTouchTeam = scoringTeam;
            state_ = MatchState::Paused;
            return;
        }
        const int defendingTeam = ball_.position.x > 0.0f ? 1 : 0;
        const int attackingTeam = 1 - defendingTeam;
        event_ = ball_.lastTouchTeam == defendingTeam ? MatchEvent::Corner : MatchEvent::GoalKick;
        restartTeam_ = event_ == MatchEvent::Corner ? attackingTeam : defendingTeam;
        restartPosition_ = {goalLine, std::copysign(kPitchHalfWidth, ball_.position.y)};
        ball_.position = restartPosition_;
        ball_.velocity = {};
        ball_.height = 0.0f;
        ball_.verticalVelocity = 0.0f;
        ball_.owner = -1;
        state_ = MatchState::Paused;
        return;
    }
    if (std::fabs(ball_.position.x) <= kPitchHalfLength && std::fabs(ball_.position.y) > kPitchHalfWidth) {
        event_ = MatchEvent::ThrowIn;
        restartTeam_ = ball_.lastTouchTeam < 0 ? 1 : 1 - ball_.lastTouchTeam;
        restartPosition_ = {std::max(-kPitchHalfLength, std::min(kPitchHalfLength, ball_.position.x)), std::copysign(kPitchHalfWidth, ball_.position.y)};
        ball_.position = restartPosition_;
        ball_.velocity = {};
        ball_.height = 0.0f;
        ball_.verticalVelocity = 0.0f;
        ball_.owner = -1;
        state_ = MatchState::Paused;
        return;
    }
    if (std::fabs(ball_.position.x) <= kPitchHalfLength && std::fabs(ball_.position.y) <= kPitchHalfWidth) {
        for (int team = 0; team < 2; ++team) {
            const int keeperId = team * 11;
            Player& keeper = players_[keeperId];
            if (!keeper.sentOff && distance(keeper.position, ball_.position) <= 2.35f && ball_.height <= 3.0f) {
                const bool highBall = ball_.height > 1.2f;
                ball_.owner = keeperId; ball_.lastTouchTeam = team; ball_.velocity = {};
                ball_.height = 0.0f; ball_.verticalVelocity = 0.0f;
                keeper.animation = highBall ? PlayerAnimation::KeeperHighCatch : PlayerAnimation::KeeperCatch;
                keeper.animationSeconds = 0.7f;
                return;
            }
        }
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
    const float dt = std::min(deltaSeconds, 0.05f) * timeScale_;
    matchSeconds_ += dt;
    updateTeam(dt, 0);
    updateTeam(dt, 1);
    updateAutonomousDecision(dt);
    updateBall(dt);
    if (advantageActive_) {
        if (event_ != MatchEvent::Advantage) {
            const int offender = advantagePlayer_;
            const bool advantageSucceeded = (event_ == MatchEvent::Goal && ball_.lastTouchTeam == advantageTeam_) ||
                (event_ != MatchEvent::Goal && restartTeam_ == advantageTeam_);
            if (advantageSucceeded) {
                advantageActive_ = false;
                advantageTeam_ = -1;
                advantagePlayer_ = -1;
                advantageSeconds_ = 0.0f;
            } else {
                if (event_ == MatchEvent::Goal) {
                    scores_[0] = advantageScoreBefore_[0];
                    scores_[1] = advantageScoreBefore_[1];
                }
                advantageActive_ = false;
                registerFoul(offender);
            }
        } else if (ball_.owner >= 0 && players_[ball_.owner].team == advantageTeam_) {
            advantageActive_ = false;
            advantageTeam_ = -1;
            advantagePlayer_ = -1;
            advantageSeconds_ = 0.0f;
            clearEvent();
        } else {
            advantageSeconds_ = std::max(0.0f, advantageSeconds_ - dt);
            if (advantageSeconds_ <= 0.0f) {
                const int offender = advantagePlayer_;
                advantageActive_ = false;
                registerFoul(offender);
            }
        }
    }
    updateReferee(dt);
    if (cinematicSlowMoRemaining_ > 0.0f) {
        cinematicSlowMoRemaining_ = std::max(0.0f, cinematicSlowMoRemaining_ - dt);
        if (cinematicSlowMoRemaining_ == 0.0f) timeScale_ = previousTimeScale_;
    }
}
bool MatchSimulation::command(int playerId, PlayerAction action, int targetPlayerId,
                              float directionX, float directionY) {
    return executeCommand(playerId, action, targetPlayerId, directionX, directionY, false);
}

bool MatchSimulation::executeCommand(int playerId, PlayerAction action, int targetPlayerId,
                                     float directionX, float directionY, bool fromAI) {
    if (state_ != MatchState::Running || playerId < 0 || playerId >= kPlayerCount) return false;
    Player& player = players_[playerId];
    if (player.sentOff || (!fromAI && (controlRole_ != ControlRole::Player || playerId != controlledPlayer_ || (spectatorMode_ && player.team == 0)))) return false;
    const Vec2 direction = normalized({directionX, directionY});
    switch (action) {
        case PlayerAction::Pass:
            if (ball_.owner != playerId) return false;
            if (targetPlayerId < 0) {
                float best = 1.0e9f;
                for (int i = player.team * 11; i < player.team * 11 + 11; ++i) {
                    if (i == playerId || players_[i].sentOff) continue;
                    const Vec2 delta{players_[i].position.x - player.position.x, players_[i].position.y - player.position.y};
                    const float forward = delta.x * direction.x + delta.y * direction.y;
                    if (forward <= 0.0f) continue;
                    const float d = distance(players_[i].position, player.position);
                    if (d < best) { best = d; targetPlayerId = i; }
                }
            }
            if (targetPlayerId < 0) {
                float best = 1.0e9f;
                for (int i = player.team * 11; i < player.team * 11 + 11; ++i) {
                    if (i == playerId || players_[i].sentOff) continue;
                    const float d = distance(players_[i].position, player.position);
                    if (d < best) { best = d; targetPlayerId = i; }
                }
            }
            if (targetPlayerId < 0 || targetPlayerId >= kPlayerCount || players_[targetPlayerId].team != player.team || players_[targetPlayerId].sentOff) return false;
            if (detectOffside(player.team, targetPlayerId)) {
                event_ = MatchEvent::Offside;
                eventPlayer_ = targetPlayerId;
                restartTeam_ = 1 - player.team;
                restartPosition_ = ball_.position;
                ball_.owner = -1;
                ball_.velocity = {};
                state_ = MatchState::Paused;
                previousTimeScale_ = timeScale_;
                timeScale_ = std::min(timeScale_, 0.35f);
                cinematicSlowMoRemaining_ = 1.0f;
                return true;
            }
            ball_.owner = -1;
            ball_.lastTouchTeam = player.team;
            ball_.position = player.position;
            ball_.velocity = {direction.x * 17.0f, direction.y * 17.0f};
            ball_.height = 0.25f; ball_.verticalVelocity = 1.0f;
            return true;
        case PlayerAction::Shoot:
            if (ball_.owner != playerId) return false;
            ball_.owner = -1;
            ball_.lastTouchTeam = player.team;
            ball_.position = player.position;
            ball_.velocity = {direction.x * 25.0f, direction.y * 25.0f};
            ball_.height = 0.35f; ball_.verticalVelocity = 4.2f;
            return true;
        case PlayerAction::Dribble:
        case PlayerAction::Sprint:
            if (action == PlayerAction::Sprint && player.stamina <= 0.0f) return false;
            playerInput_ = direction;
            playerSpeedMultiplier_ = action == PlayerAction::Sprint ? 1.45f : 1.05f;
            player.velocity = {direction.x * (action == PlayerAction::Sprint ? 9.0f : 6.5f),
                               direction.y * (action == PlayerAction::Sprint ? 9.0f : 6.5f)};
            return true;
        case PlayerAction::KeeperDive: {
            if (player.role != 0 || player.animation == PlayerAnimation::KeeperDive ||
                player.animation == PlayerAnimation::KeeperFall || player.animation == PlayerAnimation::KeeperRecover) return false;
            const Vec2 toBall{ball_.position.x - player.position.x, ball_.position.y - player.position.y};
            if (distance(player.position, ball_.position) > 10.0f) return false;
            const Vec2 dive = normalized(toBall);
            player.velocity = {dive.x * 10.0f, dive.y * 10.0f};
            player.animation = PlayerAnimation::KeeperDive;
            player.animationSeconds = 0.35f;
            return true;
        }
        case PlayerAction::KeeperCatch: {
            if (player.role != 0 || distance(player.position, ball_.position) > 4.2f || ball_.height > 3.4f) return false;
            const bool highBall = ball_.height > 1.2f;
            ball_.owner = playerId; ball_.lastTouchTeam = player.team; ball_.velocity = {};
            ball_.height = 0.0f; ball_.verticalVelocity = 0.0f;
            player.animation = highBall ? PlayerAnimation::KeeperHighCatch : PlayerAnimation::KeeperCatch;
            player.animationSeconds = 0.75f;
            return true;
        }
        case PlayerAction::Tackle: {
            const int opponent = nearestPlayer(1 - player.team, player.position);
            if (opponent < 0 || distance(player.position, players_[opponent].position) > 2.2f) return false;
            if (ball_.owner == opponent) {
                const float speed = std::sqrt(player.velocity.x * player.velocity.x + player.velocity.y * player.velocity.y);
                if (speed > 7.5f) { registerFoul(playerId); return false; }
                ball_.owner = playerId;
                ball_.lastTouchTeam = player.team;
                return true;
            }
            return false;
        }
    }
    return false;
}
bool MatchSimulation::issueCard(int playerId, int cardType) {
    if (controlRole_ != ControlRole::Referee || playerId < 0 || playerId >= kPlayerCount || players_[playerId].sentOff) return false;
    if (cardType == 1) {
        ++players_[playerId].yellowCards;
        if (players_[playerId].yellowCards >= 2) return setPlayerSentOff(playerId, true);
        players_[playerId].animation = PlayerAnimation::Protest;
        players_[playerId].animationSeconds = 1.35f;
        players_[playerId].protestSeconds = 1.35f;
        return true;
    }
    if (cardType == 2) return setPlayerSentOff(playerId, true);
    return false;
}
bool MatchSimulation::setPlayerSentOff(int playerId, bool sentOff) {
    if (playerId < 0 || playerId >= kPlayerCount) return false;
    players_[playerId].sentOff = sentOff;
    if (sentOff) {
        players_[playerId].protestSeconds = 1.8f;
        players_[playerId].animation = PlayerAnimation::Dismissal;
        players_[playerId].animationSeconds = 1.8f;
    } else {
        players_[playerId].animation = PlayerAnimation::None;
        players_[playerId].animationSeconds = 0.0f;
        players_[playerId].protestSeconds = 0.0f;
    }
    if (sentOff && ball_.owner == playerId) ball_.owner = -1;
    return true;
}


void MatchSimulation::setTimeScale(float scale) {
    if (!std::isfinite(scale)) return;
    timeScale_ = std::max(0.15f, std::min(2.0f, scale));
}
void MatchSimulation::updateReferee(float dt) {
    Vec2 offset{0.0f, -5.0f};
    if (ball_.velocity.x != 0.0f || ball_.velocity.y != 0.0f) {
        const Vec2 movement = normalized(ball_.velocity);
        offset = {-movement.x * 4.5f - movement.y * 2.0f,
                  -movement.y * 4.5f + movement.x * 2.0f};
    }
    Vec2 target{ball_.position.x + offset.x, ball_.position.y + offset.y};
    target.x = std::max(-34.0f, std::min(34.0f, target.x));
    target.y = std::max(-53.0f, std::min(53.0f, target.y));
    const Vec2 direction = normalized({target.x - refereePosition_.x, target.y - refereePosition_.y});
    const float distanceToTarget = distance(target, refereePosition_);
    const float step = std::min(distanceToTarget, 5.0f * dt);
    refereePosition_.x += direction.x * step;
    refereePosition_.y += direction.y * step;
}
bool MatchSimulation::detectOffside(int attackingTeam, int receiverId) {
    if (receiverId < 0 || receiverId >= kPlayerCount) return false;
    const float sign = attackingTeam == 0 ? 1.0f : -1.0f;
    float defenders[11]{};
    int count = 0;
    for (int i = (1 - attackingTeam) * 11; i < (1 - attackingTeam) * 11 + 11; ++i) {
        if (players_[i].sentOff) continue;
        defenders[count++] = players_[i].position.x * sign;
    }
    if (count < 2) return false;
    std::sort(defenders, defenders + count, std::greater<float>());
    offsideLine_ = defenders[1] * sign;
    const Player& receiver = players_[receiverId];
    const float receiverProgress = receiver.position.x * sign;
    const float ballProgress = ball_.position.x * sign;
    const bool inOpponentHalf = receiverProgress > 0.0f;
    const bool aheadOfBall = receiverProgress > ballProgress + 0.15f;
    const bool aheadOfSecondLast = receiverProgress > defenders[1] + 0.25f;
    const bool offside = inOpponentHalf && aheadOfBall && aheadOfSecondLast;
    offsideLineVisible_ = offside;
    return offside;
}
void MatchSimulation::registerFoul(int playerId) {
    if (playerId < 0 || playerId >= kPlayerCount) return;
    eventPlayer_ = playerId;
    offsideLineVisible_ = false;
    ball_.owner = -1;
    ball_.velocity = {};
    const int attackingTeam = 1 - players_[playerId].team;
    const bool inPenaltyArea = attackingTeam == 0
        ? (ball_.position.x > 25.0f && std::fabs(ball_.position.y) < 16.0f)
        : (ball_.position.x < -25.0f && std::fabs(ball_.position.y) < 16.0f);
    event_ = inPenaltyArea ? MatchEvent::Penalty : MatchEvent::Foul;
    restartTeam_ = attackingTeam;
    restartPosition_ = ball_.position;
    state_ = MatchState::Paused;
    previousTimeScale_ = timeScale_;
    timeScale_ = std::min(timeScale_, 0.35f);
    cinematicSlowMoRemaining_ = 1.1f;
}
bool MatchSimulation::callFoul(int playerId) {
    if (controlRole_ != ControlRole::Referee || playerId < 0 || playerId >= kPlayerCount || players_[playerId].sentOff) return false;
    if (state_ != MatchState::Running) return false;
    registerFoul(playerId);
    return true;
}
bool MatchSimulation::playAdvantage(int playerId) {
    if (controlRole_ != ControlRole::Referee || state_ != MatchState::Running || advantageActive_ || playerId < 0 || playerId >= kPlayerCount || players_[playerId].sentOff) return false;
    advantageActive_ = true;
    advantageTeam_ = 1 - players_[playerId].team;
    advantagePlayer_ = playerId;
    advantageSeconds_ = 3.0f;
    advantageScoreBefore_[0] = scores_[0];
    advantageScoreBefore_[1] = scores_[1];
    event_ = MatchEvent::Advantage;
    eventPlayer_ = playerId;
    return true;
}
bool MatchSimulation::callOffside(int playerId) {
    if (controlRole_ != ControlRole::Referee || playerId < 0 || playerId >= kPlayerCount || state_ == MatchState::Finished || state_ == MatchState::Ready) return false;
    event_ = MatchEvent::Offside;
    offsideLineVisible_ = true;
    eventPlayer_ = playerId;
    restartTeam_ = 1 - players_[playerId].team;
    restartPosition_ = ball_.position;
    offsideLine_ = players_[playerId].position.x;
    offsideLineVisible_ = true;
    ball_.owner = -1;
    ball_.velocity = {};
    state_ = MatchState::Paused;
    return true;
}
bool MatchSimulation::requestVarReview() {
    if (controlRole_ != ControlRole::Referee || state_ != MatchState::Paused || event_ == MatchEvent::None || event_ == MatchEvent::VarReview) return false;
    eventBeforeVar_ = event_;
    ballBeforeVar_ = event_ == MatchEvent::Goal ? ballBeforeGoal_ : ball_;
    if (event_ == MatchEvent::Goal) {
        scoresBeforeVar_[0] = scoresBeforeGoal_[0];
        scoresBeforeVar_[1] = scoresBeforeGoal_[1];
    } else {
        scoresBeforeVar_[0] = scores_[0];
        scoresBeforeVar_[1] = scores_[1];
    }
    event_ = MatchEvent::VarReview;
    return true;
}
bool MatchSimulation::resolveVar(bool upholdDecision) {
    if (controlRole_ != ControlRole::Referee || state_ != MatchState::Paused || event_ != MatchEvent::VarReview) return false;
    if (upholdDecision) {
        event_ = eventBeforeVar_;
    } else if (eventBeforeVar_ == MatchEvent::Goal) {
        ball_ = ballBeforeVar_;
        scores_[0] = scoresBeforeVar_[0];
        scores_[1] = scoresBeforeVar_[1];
        const int defendingTeam = ballBeforeVar_.position.x > 0.0f ? 1 : 0;
        event_ = MatchEvent::GoalKick;
        restartTeam_ = defendingTeam;
        restartPosition_ = {defendingTeam == 0 ? -29.5f : 29.5f, 0.0f};
        ball_.position = restartPosition_;
        ball_.velocity = {};
        ball_.height = 0.0f;
        ball_.verticalVelocity = 0.0f;
        ball_.owner = -1;
        eventPlayer_ = -1;
        state_ = MatchState::Paused;
    } else {
        ball_ = ballBeforeVar_;
        scores_[0] = scoresBeforeVar_[0];
        scores_[1] = scoresBeforeVar_[1];
        clearEvent();
        state_ = MatchState::Running;
    }
    eventBeforeVar_ = MatchEvent::None;
    return true;
}
bool MatchSimulation::restartPlay() {
    if (state_ != MatchState::Paused || restartTeam_ < 0 || event_ == MatchEvent::VarReview) return false;
    const MatchEvent restartEvent = event_;
    const float sign = restartPosition_.x >= 0.0f ? 1.0f : -1.0f;
    int taker = -1;
    if (restartEvent == MatchEvent::Penalty) {
        restartPosition_ = {restartTeam_ == 0 ? 29.5f : -29.5f, 0.0f};
        taker = nearestPlayer(restartTeam_, restartPosition_);
    } else if (restartEvent == MatchEvent::GoalKick) {
        restartPosition_ = {restartTeam_ == 0 ? -29.5f : 29.5f, 0.0f};
        const int keeper = restartTeam_ * 11;
        taker = players_[keeper].sentOff ? nearestPlayer(restartTeam_, restartPosition_) : keeper;
    } else if (restartEvent == MatchEvent::Corner) {
        restartPosition_ = {sign * (kPitchHalfLength - 0.5f), std::copysign(kPitchHalfWidth - 0.5f, restartPosition_.y)};
        taker = nearestPlayer(restartTeam_, restartPosition_);
    } else if (restartEvent == MatchEvent::ThrowIn) {
        restartPosition_.x = std::max(-kPitchHalfLength + 0.5f, std::min(kPitchHalfLength - 0.5f, restartPosition_.x));
        restartPosition_.y = std::copysign(kPitchHalfWidth - 0.5f, restartPosition_.y);
        taker = nearestPlayer(restartTeam_, restartPosition_);
    } else if (restartEvent == MatchEvent::Goal) {
        restartPosition_ = {};
        taker = nearestPlayer(restartTeam_, restartPosition_);
    } else taker = nearestPlayer(restartTeam_, restartPosition_);
    if (taker < 0 || players_[taker].sentOff) return false;
    const float attackDirection = restartTeam_ == 0 ? 1.0f : -1.0f;
    players_[taker].position = {restartPosition_.x - attackDirection * 0.9f, restartPosition_.y};
    ball_.owner = taker;
    ball_.position = restartPosition_;
    ball_.velocity = {};
    ball_.height = 0.0f;
    ball_.verticalVelocity = 0.0f;
    ball_.lastTouchTeam = restartTeam_;
    advantageActive_ = false;
    advantageTeam_ = -1;
    advantagePlayer_ = -1;
    advantageSeconds_ = 0.0f;
    clearEvent();
    state_ = MatchState::Running;
    return true;
}
void MatchSimulation::clearEvent() {
    event_ = MatchEvent::None;
    eventBeforeVar_ = MatchEvent::None;
    ballBeforeVar_ = {};
    eventPlayer_ = -1;
    restartTeam_ = -1;
    offsideLineVisible_ = false;
}

} // namespace football

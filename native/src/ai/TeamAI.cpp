#include "football/ai/TeamAI.hpp"
#include <algorithm>
#include <cmath>

namespace football {
namespace {
constexpr Vec2 kHomeShape[11] = {
    {-32.0f, 0.0f},
    {-24.0f, -35.0f}, {-25.0f, -12.0f}, {-25.0f, 12.0f}, {-24.0f, 35.0f},
    {-14.0f, -20.0f}, {-15.0f, 0.0f}, {-14.0f, 20.0f},
    {-5.0f, -30.0f}, {-5.0f, 30.0f}, {-2.0f, 0.0f}
};

float clamp(float value, float low, float high) {
    return std::max(low, std::min(high, value));
}
}

Vec2 TeamAI::targetForPlayer(const TeamAIContext& c) {
    if (c.team < 0 || c.team > 1 || c.role < 0 || c.role >= 11) return c.playerPosition;
    const float direction = c.team == 0 ? 1.0f : -1.0f;
    const float ballProgress = c.ballPosition.x * direction;
    const bool inPossession = c.possessionTeam == c.team;
    const bool pressing = !inPossession && c.playerId == c.chaserId;
    const Vec2 home = kHomeShape[c.role];

    if (c.role == 0) {
        float keeperProgress = -32.0f;
        if (ballProgress < -18.0f) keeperProgress += 1.5f;
        return {keeperProgress * direction, clamp(c.ballPosition.y * 0.32f, -12.0f, 12.0f)};
    }

    if (pressing) {
        return {clamp(c.ballPosition.x - direction * 0.8f, -34.0f, 34.0f),
                clamp(c.ballPosition.y, -53.0f, 53.0f)};
    }

    float targetProgress = home.x;
    float targetY = home.y;

    if (c.role <= 4) {
        float defensiveLine = inPossession
            ? -23.0f + clamp(ballProgress * 0.10f, -3.0f, 7.0f)
            : -27.0f + clamp(ballProgress * 0.12f, -3.0f, 5.0f);
        if (c.style == TacticalStyle::HighPress) defensiveLine += 3.0f;
        if (c.style == TacticalStyle::DefensiveBlock) defensiveLine -= 4.0f;
        targetProgress = defensiveLine;
        const float width = c.style == TacticalStyle::Possession ? 0.96f : 0.86f;
        targetY = home.y * width + c.ballPosition.y * (inPossession ? 0.08f : 0.12f);
    } else if (c.role <= 7) {
        const float supportDepth = c.style == TacticalStyle::CounterAttack ? 7.0f : 5.0f;
        targetProgress = inPossession
            ? ballProgress - supportDepth
            : ballProgress - (c.role == 6 ? 5.0f : 9.0f);
        targetProgress = clamp(targetProgress, -29.0f, 24.0f);
        const float width = c.style == TacticalStyle::Possession ? 1.12f : 0.78f;
        targetY = home.y * width + c.ballPosition.y * (inPossession ? 0.16f : 0.10f);
    } else {
        if (inPossession) {
            const float runAhead = c.style == TacticalStyle::CounterAttack ? 12.0f :
                                   c.style == TacticalStyle::Possession ? 6.0f : 8.5f;
            targetProgress = std::max(home.x + ballProgress * 0.20f, ballProgress + runAhead);
        } else {
            const float pressDepth = c.style == TacticalStyle::HighPress ? 5.0f :
                                     c.style == TacticalStyle::DefensiveBlock ? 15.0f : 10.0f;
            targetProgress = ballProgress + pressDepth;
        }
        targetProgress = clamp(targetProgress, -25.0f, 32.0f);
        const float width = c.style == TacticalStyle::Possession ? 1.15f : 0.92f;
        targetY = home.y * width + c.ballPosition.y * (inPossession ? 0.08f : 0.04f);
        if (inPossession && c.role == 10) {
            targetProgress += 2.0f;
            targetY = clamp(c.ballPosition.y * 0.18f, -7.0f, 7.0f);
        } else if (inPossession && (c.role == 8 || c.role == 9)) {
            const float flank = c.role == 8 ? -1.0f : 1.0f;
            if (c.ballPosition.y * flank < -8.0f) {
                targetY = flank * 9.0f;
                targetProgress += 2.5f;
            } else {
                targetY = flank * 27.0f + c.ballPosition.y * 0.08f;
            }
        }
    }

    if (!inPossession && c.hasMarkingTarget && c.role >= 1 && c.role <= 7) {
        targetProgress = targetProgress * 0.34f + c.markingTarget.x * direction * 0.66f;
        targetY = targetY * 0.34f + c.markingTarget.y * 0.66f;
    }
    if (!inPossession && c.hasCoverTarget && (c.role == 2 || c.role == 3)) {
        targetProgress = targetProgress * 0.72f + c.coverTarget.x * direction * 0.28f;
        targetY = targetY * 0.72f + c.coverTarget.y * 0.28f;
    }

    return {clamp(targetProgress * direction, -35.0f, 35.0f),
            clamp(targetY, -53.0f, 53.0f)};
}

float TeamAI::movementSpeedMultiplier(const TeamAIContext& c) {
    switch (c.style) {
        case TacticalStyle::HighPress: return 1.08f;
        case TacticalStyle::CounterAttack: return c.possessionTeam == c.team ? 1.05f : 0.98f;
        case TacticalStyle::DefensiveBlock: return 0.97f;
        case TacticalStyle::Possession:
        case TacticalStyle::Balanced:
        default: return 1.0f;
    }
}


AIBallAction TeamAI::decideBallAction(const AIBallDecisionContext& c) {
    if (c.team < 0 || c.team > 1 || c.role < 0 || c.role >= 11)
        return AIBallAction::Pass;
    const float direction = c.team == 0 ? 1.0f : -1.0f;
    const float progress = c.playerPosition.x * direction;
    const float goalDistance = 36.0f - progress;
    const float lateralDistance = std::fabs(c.playerPosition.y);
    const float shootingLimit = c.difficulty == AIDifficulty::Hard ? 22.0f :
                                c.difficulty == AIDifficulty::Easy ? 14.0f : 18.0f;
    const float shotSeparation = c.difficulty == AIDifficulty::Hard ? 1.2f :
                                 c.difficulty == AIDifficulty::Easy ? 3.0f : 1.8f;
    if (c.wideChannel && c.teammateInBox && goalDistance <= 30.0f &&
        goalDistance >= 7.0f && c.nearestOpponentDistance >= shotSeparation)
        return AIBallAction::Cross;
    const bool shootingRange = goalDistance <= shootingLimit && lateralDistance <= 19.0f;
    const bool shotWindow = !c.forwardLaneBlocked || goalDistance <= 11.0f;
    if (shootingRange && shotWindow && c.nearestOpponentDistance >= shotSeparation)
        return AIBallAction::Shoot;
    const float minimumProgress = c.difficulty == AIDifficulty::Hard ? 1.0f :
                                  c.difficulty == AIDifficulty::Easy ? 4.0f : 2.0f;
    const bool forwardOption = c.bestTeammateId >= 0 &&
        c.bestTeammateProgress >= minimumProgress && c.bestTeammatePressure >= 2.0f;
    const bool underPressure = c.nearestOpponentDistance < 5.0f;
    if (forwardOption && (underPressure || c.forwardLaneBlocked ||
                          goalDistance > 21.0f ||
                          c.style == TacticalStyle::Possession))
        return AIBallAction::Pass;
    if (c.nearestOpponentDistance >= 2.4f && goalDistance > 8.0f &&
        !c.forwardLaneBlocked)
        return AIBallAction::Dribble;
    if (c.bestTeammateId >= 0) return AIBallAction::Pass;
    return AIBallAction::Dribble;
}

} // namespace football

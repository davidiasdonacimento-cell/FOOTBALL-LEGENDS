#pragma once
#include "football/core/MatchSimulation.hpp"

namespace football {

struct TeamAIContext {
    int team{0};
    int playerId{-1};
    int role{0};
    Vec2 playerPosition{};
    Vec2 ballPosition{};
    int possessionTeam{-1};
    int chaserId{-1};
    float stamina{1.0f};
    TacticalStyle style{TacticalStyle::Balanced};
};

enum class AIBallAction : std::uint8_t { Pass, Shoot, Dribble };

struct AIBallDecisionContext {
    int team{0};
    int role{0};
    Vec2 playerPosition{};
    int bestTeammateId{-1};
    float bestTeammateProgress{0.0f};
    float bestTeammatePressure{0.0f};
    float nearestOpponentDistance{1000.0f};
    bool forwardLaneBlocked{false};
    TacticalStyle style{TacticalStyle::Balanced};
};

class TeamAI final {
public:
    static Vec2 targetForPlayer(const TeamAIContext& context);
    static float movementSpeedMultiplier(const TeamAIContext& context);
    static AIBallAction decideBallAction(const AIBallDecisionContext& context);
};

} // namespace football

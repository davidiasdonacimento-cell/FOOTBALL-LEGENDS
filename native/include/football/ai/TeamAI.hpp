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

class TeamAI final {
public:
    static Vec2 targetForPlayer(const TeamAIContext& context);
    static float movementSpeedMultiplier(const TeamAIContext& context);
};

} // namespace football

#include "football/ai/TeamAI.hpp"
#include <cassert>
#include <cmath>

using namespace football;

static bool near(float a, float b, float epsilon = 0.001f) {
    return std::fabs(a - b) <= epsilon;
}

int main() {
    TeamAIContext forward{0, 9, 9, {-5.0f, -30.0f}, {0.0f, 0.0f}, 0, -1, 1.0f, TacticalStyle::Balanced};
    const Vec2 balancedRun = TeamAI::targetForPlayer(forward);
    assert(balancedRun.x > 0.0f);
    assert(balancedRun.y < 0.0f);

    forward.style = TacticalStyle::CounterAttack;
    const Vec2 counterRun = TeamAI::targetForPlayer(forward);
    assert(counterRun.x > balancedRun.x);

    TeamAIContext highPress{1, 13, 2, {-20.0f, 10.0f}, {4.0f, 8.0f}, 0, 13, 0.8f, TacticalStyle::HighPress};
    const Vec2 pressTarget = TeamAI::targetForPlayer(highPress);
    assert(near(pressTarget.x, 4.8f));
    assert(near(pressTarget.y, 8.0f));
    assert(TeamAI::movementSpeedMultiplier(highPress) > 1.0f);

    TeamAIContext deepBlock = highPress;
    deepBlock.playerId = 14;
    deepBlock.role = 3;
    deepBlock.chaserId = -1;
    deepBlock.style = TacticalStyle::DefensiveBlock;
    const Vec2 blockTarget = TeamAI::targetForPlayer(deepBlock);
    TeamAIContext advancedLine = deepBlock;
    advancedLine.style = TacticalStyle::HighPress;
    const Vec2 highLineTarget = TeamAI::targetForPlayer(advancedLine);
    assert(highLineTarget.x < blockTarget.x);

    TeamAIContext keeper{0, 0, 0, {-32.0f, 0.0f}, {10.0f, 20.0f}, 1, 0, 1.0f, TacticalStyle::Balanced};
    const Vec2 keeperTarget = TeamAI::targetForPlayer(keeper);
    assert(near(keeperTarget.x, -32.0f));
    assert(near(keeperTarget.y, 6.4f));

    TeamAIContext mirrored{1, 20, 9, {5.0f, -30.0f}, {0.0f, 0.0f}, 1, -1, 1.0f, TacticalStyle::Balanced};
    const Vec2 mirroredRun = TeamAI::targetForPlayer(mirrored);
    assert(near(mirroredRun.x, -balancedRun.x));
    assert(near(mirroredRun.y, balancedRun.y));

    for (int team = 0; team < 2; ++team) {
        for (int role = 0; role < 11; ++role) {
            TeamAIContext input{team, team * 11 + role, role, {0.0f, 0.0f}, {33.0f, -52.0f},
                                team, -1, 0.5f, TacticalStyle::Possession};
            const Vec2 target = TeamAI::targetForPlayer(input);
            assert(std::isfinite(target.x) && std::isfinite(target.y));
            assert(target.x >= -35.0f && target.x <= 35.0f);
            assert(target.y >= -53.0f && target.y <= 53.0f);
        }
    }

    MatchSimulation match;
    assert(match.tacticalStyle(0) == TacticalStyle::Balanced);
    assert(match.setTacticalStyle(0, TacticalStyle::HighPress));
    assert(match.tacticalStyle(0) == TacticalStyle::HighPress);
    assert(!match.setTacticalStyle(2, TacticalStyle::Balanced));
    assert(match.setTacticalStyle(1, TacticalStyle::DefensiveBlock));
    match.reset();
    assert(match.tacticalStyle(0) == TacticalStyle::Balanced);
    assert(match.tacticalStyle(1) == TacticalStyle::Balanced);
    return 0;
}

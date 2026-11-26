#include "football/ai/TeamAI.hpp"
#include <cassert>
#include <cmath>

using namespace football;

static bool near(float a, float b, float epsilon = 0.001f) {
    return std::fabs(a - b) <= epsilon;
}

int main() {
    TeamAIContext forward{0, 8, 8, {-5.0f, -30.0f}, {0.0f, 0.0f}, 0, -1, 1.0f, TacticalStyle::Balanced};
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

    TeamAIContext mirrored{1, 19, 8, {5.0f, -30.0f}, {0.0f, 0.0f}, 1, -1, 1.0f, TacticalStyle::Balanced};
    const Vec2 mirroredRun = TeamAI::targetForPlayer(mirrored);
    assert(near(mirroredRun.x, -balancedRun.x));
    assert(near(mirroredRun.y, balancedRun.y));

    TeamAIContext rightForward = forward;
    rightForward.playerId = 9;
    rightForward.role = 9;
    const Vec2 rightRun = TeamAI::targetForPlayer(rightForward);
    assert(rightRun.y > 0.0f);
    assert(near(rightRun.y, -balancedRun.y));

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


    football::AIBallDecisionContext shotDecision{0, 10, {24.0f, 0.0f}, 9, 5.0f, 8.0f, 6.0f, false,
                                                   football::TacticalStyle::Balanced};
    assert(football::TeamAI::decideBallAction(shotDecision) == football::AIBallAction::Shoot);
    football::AIBallDecisionContext passDecision{0, 6, {0.0f, 0.0f}, 8, 9.0f, 7.0f, 2.5f, true,
                                                   football::TacticalStyle::Balanced};
    assert(football::TeamAI::decideBallAction(passDecision) == football::AIBallAction::Pass);
    football::AIBallDecisionContext dribbleDecision{1, 9, {-5.0f, 2.0f}, -1, 0.0f, 0.0f, 8.0f, false,
                                                      football::TacticalStyle::CounterAttack};
    assert(football::TeamAI::decideBallAction(dribbleDecision) == football::AIBallAction::Dribble);
    football::AIBallDecisionContext crossDecision{0, 9, {15.0f, -30.0f}, 8, 10.0f, 6.0f, 5.0f, false,
                                                    football::TacticalStyle::Balanced, true, true,
                                                    football::AIDifficulty::Normal};
    assert(football::TeamAI::decideBallAction(crossDecision) == football::AIBallAction::Cross);
    football::AIBallDecisionContext easyShot = shotDecision;
    easyShot.difficulty = football::AIDifficulty::Easy;
    assert(football::TeamAI::decideBallAction(easyShot) != football::AIBallAction::Shoot);
    football::AIBallDecisionContext hardShot = shotDecision;
    hardShot.difficulty = football::AIDifficulty::Hard;
    assert(football::TeamAI::decideBallAction(hardShot) == football::AIBallAction::Shoot);

    TeamAIContext marking = highPress;
    marking.team = 0; marking.playerId = 2; marking.role = 2;
    marking.possessionTeam = 1; marking.chaserId = -1;
    marking.playerPosition = {-24.0f, -12.0f};
    marking.hasMarkingTarget = true; marking.markingTarget = {8.0f, -10.0f};
    const Vec2 markTarget = TeamAI::targetForPlayer(marking);
    assert(markTarget.x > -5.0f);
    assert(markTarget.y < 0.0f);
    marking.hasCoverTarget = true; marking.coverTarget = {0.0f, 4.0f};
    const Vec2 coverTarget = TeamAI::targetForPlayer(marking);
    assert(coverTarget.y > markTarget.y);

    football::AIBallDecisionContext invalidDecision{-1, 20, {}, -1, 0.0f, 0.0f, 0.0f, false,
                                                      football::TacticalStyle::Balanced};
    assert(football::TeamAI::decideBallAction(invalidDecision) == football::AIBallAction::Pass);


    MatchSimulation match;
    assert(match.tacticalStyle(0) == TacticalStyle::Balanced);
    assert(match.setTacticalStyle(0, TacticalStyle::HighPress));
    assert(match.tacticalStyle(0) == TacticalStyle::HighPress);
    assert(!match.setTacticalStyle(2, TacticalStyle::Balanced));
    assert(match.setTacticalStyle(1, TacticalStyle::DefensiveBlock));
    assert(match.aiDifficulty() == AIDifficulty::Normal);
    assert(match.setAIDifficulty(AIDifficulty::Hard));
    assert(match.aiDifficulty() == AIDifficulty::Hard);
    assert(!match.setAIDifficulty(static_cast<AIDifficulty>(99)));
    match.reset();
    assert(match.aiDifficulty() == AIDifficulty::Normal);
    assert(match.tacticalStyle(0) == TacticalStyle::Balanced);
    assert(match.tacticalStyle(1) == TacticalStyle::Balanced);
    return 0;
}

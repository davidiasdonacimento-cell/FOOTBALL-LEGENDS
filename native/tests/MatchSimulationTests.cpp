#include "football/core/MatchSimulation.hpp"
#include <cassert>
#include <cmath>

namespace football {
struct MatchSimulationTestAccess {
    static void setPlayer(MatchSimulation& match, int id, Vec2 position) {
        match.players_[id].position = position;
    }
    static void setBall(MatchSimulation& match, Vec2 position, int owner) {
        match.ball_.position = position;
        match.ball_.owner = owner;
        match.ball_.velocity = {};
    }
    static void setBallHeight(MatchSimulation& match, float height, float verticalVelocity = 0.0f) {
        match.ball_.height = height;
        match.ball_.verticalVelocity = verticalVelocity;
    }
    static void setBallMotion(MatchSimulation& match, Vec2 velocity) {
        match.ball_.velocity = velocity;
    }
    static void setLastTouchTeam(MatchSimulation& match, int team) { match.ball_.lastTouchTeam = team; }
    static void setAerialState(MatchSimulation& match, int id, float height, float velocity = 0.0f) {
        match.players_[id].jumpHeight = height;
        match.players_[id].jumpVelocity = velocity;
        match.players_[id].aerialCooldown = 0.5f;
    }
    static void setAerialAttributes(MatchSimulation& match, int id, float jumping, float heading,
                                   float strength, float aggression = 50.0f) {
        match.players_[id].jumping = jumping;
        match.players_[id].heading = heading;
        match.players_[id].strength = strength;
        match.players_[id].aggression = aggression;
    }
};
}

int main() {
    football::MatchSimulation passMatch;
    passMatch.start();
    assert(passMatch.command(10, football::PlayerAction::Pass, -1, 1.0f, 0.0f));
    assert(passMatch.ball().owner == -1);
    assert(passMatch.ball().velocity.x > 0.0f);

    football::MatchSimulation crossMatch;
    crossMatch.start();
    football::MatchSimulationTestAccess::setBall(crossMatch, {12.0f, -30.0f}, 10);
    assert(crossMatch.command(10, football::PlayerAction::Cross, 8, 0.8f, 0.6f));
    assert(crossMatch.ball().owner == -1);
    assert(crossMatch.ball().height > 0.0f);
    assert(crossMatch.ball().verticalVelocity > 0.0f);

    football::MatchSimulation keeperMatch;
    keeperMatch.start();
    assert(keeperMatch.selectControlledPlayer(0));
    assert(keeperMatch.setPlayerMovement(1.0f, 0.0f));
    for (int i = 0; i < 40; ++i) keeperMatch.update(0.05f);
    assert(keeperMatch.players()[0].position.x <= -26.0f);

    football::MatchSimulation onsideMatch;
    onsideMatch.start();
    football::MatchSimulationTestAccess::setPlayer(onsideMatch, 9, {18.0f, 0.0f});
    for (int id = 11; id < 22; ++id)
        football::MatchSimulationTestAccess::setPlayer(onsideMatch, id, {id == 11 ? 35.0f : 20.0f, static_cast<float>(id - 16)});
    football::MatchSimulationTestAccess::setPlayer(onsideMatch, 10, {8.0f, 0.0f});
    football::MatchSimulationTestAccess::setBall(onsideMatch, {8.0f, 0.0f}, 10);
    assert(onsideMatch.command(10, football::PlayerAction::Pass, 9, 1.0f, 0.0f));
    assert(onsideMatch.event() == football::MatchEvent::None);
    assert(!onsideMatch.offsideLineVisible());

    football::MatchSimulation keeperDive;
    keeperDive.start();
    football::MatchSimulationTestAccess::setBall(keeperDive, {-29.0f, 0.0f}, -1);
    assert(keeperDive.selectControlledPlayer(0));
    assert(keeperDive.command(0, football::PlayerAction::KeeperDive));
    assert(keeperDive.ball().owner == -1);
    assert(keeperDive.players()[0].animation == football::PlayerAnimation::KeeperDive);
    keeperDive.update(0.05f);
    assert(keeperDive.ball().owner == 0);
    assert(keeperDive.players()[0].animation == football::PlayerAnimation::KeeperCatch);
    for (int i = 0; i < 20; ++i) keeperDive.update(0.05f);
    assert(keeperDive.players()[0].animation == football::PlayerAnimation::None);

    football::MatchSimulation keeperMissedDive;
    keeperMissedDive.start();
    football::MatchSimulationTestAccess::setBall(keeperMissedDive, {-29.0f, 8.0f}, -1);
    assert(keeperMissedDive.selectControlledPlayer(0));
    assert(keeperMissedDive.command(0, football::PlayerAction::KeeperDive));
    for (int i = 0; i < 8; ++i) keeperMissedDive.update(0.05f);
    assert(keeperMissedDive.players()[0].animation == football::PlayerAnimation::KeeperFall ||
           keeperMissedDive.players()[0].animation == football::PlayerAnimation::KeeperRecover);
    for (int i = 0; i < 20; ++i) keeperMissedDive.update(0.05f);
    assert(keeperMissedDive.players()[0].animation == football::PlayerAnimation::None);

    football::MatchSimulation keeperCatch;
    keeperCatch.start();
    football::MatchSimulationTestAccess::setBall(keeperCatch, {-30.0f, 0.0f}, -1);
    assert(keeperCatch.selectControlledPlayer(0));
    assert(keeperCatch.command(0, football::PlayerAction::KeeperCatch));
    assert(keeperCatch.ball().owner == 0);
    assert(keeperCatch.players()[0].animation == football::PlayerAnimation::KeeperCatch);

    football::MatchSimulation highBallKeeper;
    highBallKeeper.start();
    football::MatchSimulationTestAccess::setBall(highBallKeeper, {-30.0f, 0.0f}, -1);
    football::MatchSimulationTestAccess::setBallHeight(highBallKeeper, 2.6f);
    assert(highBallKeeper.selectControlledPlayer(0));
    assert(highBallKeeper.command(0, football::PlayerAction::KeeperCatch));
    assert(highBallKeeper.ball().owner == 0);
    assert(highBallKeeper.ball().height == 0.0f);
    assert(highBallKeeper.players()[0].animation == football::PlayerAnimation::KeeperHighCatch);

    football::MatchSimulation validGoal;
    validGoal.start();
    football::MatchSimulationTestAccess::setBall(validGoal, {35.5f, 0.0f}, -1);
    football::MatchSimulationTestAccess::setBallMotion(validGoal, {20.0f, 0.0f});
    validGoal.update(0.05f);
    assert(validGoal.score(0) == 1);
    assert(validGoal.event() == football::MatchEvent::Goal);
    assert(validGoal.state() == football::MatchState::Paused);
    assert(validGoal.restartPlay());
    assert(validGoal.state() == football::MatchState::Running);
    assert(validGoal.ball().owner >= 11 && validGoal.ball().owner < 22);

    // The regular Continue action must execute the opponent kickoff after a goal.
    football::MatchSimulation goalResume;
    goalResume.start();
    football::MatchSimulationTestAccess::setBall(goalResume, {35.5f, 0.0f}, -1);
    football::MatchSimulationTestAccess::setBallMotion(goalResume, {20.0f, 0.0f});
    goalResume.update(0.05f);
    assert(goalResume.state() == football::MatchState::Paused);
    assert(goalResume.event() == football::MatchEvent::Goal);
    goalResume.resume();
    assert(goalResume.state() == football::MatchState::Running);
    assert(goalResume.event() == football::MatchEvent::None);
    assert(goalResume.ball().owner >= 11 && goalResume.ball().owner < 22);

    football::MatchSimulation wideShot;
    wideShot.start();
    football::MatchSimulationTestAccess::setBall(wideShot, {35.5f, 7.0f}, -1);
    football::MatchSimulationTestAccess::setBallMotion(wideShot, {20.0f, 0.0f});
    wideShot.update(0.05f);
    assert(wideShot.score(0) == 0);
    assert(wideShot.event() == football::MatchEvent::GoalKick);
    assert(wideShot.restartTeam() == 1);

    football::MatchSimulation highShot;
    highShot.start();
    football::MatchSimulationTestAccess::setBall(highShot, {35.5f, 0.0f}, -1);
    football::MatchSimulationTestAccess::setBallHeight(highShot, 2.5f);
    football::MatchSimulationTestAccess::setBallMotion(highShot, {20.0f, 0.0f});
    highShot.update(0.05f);
    assert(highShot.score(0) == 0);
    assert(highShot.event() == football::MatchEvent::GoalKick);
    assert(highShot.restartTeam() == 1);

    football::MatchSimulation postShot;
    postShot.start();
    football::MatchSimulationTestAccess::setBall(postShot, {35.9f, 3.66f}, -1);
    football::MatchSimulationTestAccess::setBallMotion(postShot, {4.0f, 0.0f});
    postShot.update(0.05f);
    assert(postShot.score(0) == 0);
    assert(postShot.ball().velocity.x < 0.0f);

    football::MatchSimulation airborneBall;
    airborneBall.start();
    football::MatchSimulationTestAccess::setBall(airborneBall, {0.0f, 0.0f}, -1);
    football::MatchSimulationTestAccess::setBallHeight(airborneBall, 1.0f, 3.0f);
    airborneBall.update(0.05f);
    assert(airborneBall.ball().height > 1.0f);

    football::MatchSimulation jumpTest;
    jumpTest.start();
    football::MatchSimulationTestAccess::setAerialAttributes(jumpTest, 10, 95.0f, 60.0f, 60.0f);
    assert(jumpTest.command(10, football::PlayerAction::Jump));
    assert(jumpTest.players()[10].jumpVelocity > 6.3f);
    jumpTest.update(0.05f);
    assert(jumpTest.players()[10].jumpHeight > 0.25f);
    assert(jumpTest.players()[10].animation == football::PlayerAnimation::Jump);

    football::MatchSimulation headerLow;
    football::MatchSimulation headerHigh;
    headerLow.start();
    headerHigh.start();
    football::MatchSimulationTestAccess::setPlayer(headerLow, 10, {0.0f, 0.0f});
    football::MatchSimulationTestAccess::setPlayer(headerHigh, 10, {0.0f, 0.0f});
    football::MatchSimulationTestAccess::setBall(headerLow, {0.8f, 0.0f}, -1);
    football::MatchSimulationTestAccess::setBall(headerHigh, {0.8f, 0.0f}, -1);
    football::MatchSimulationTestAccess::setBallHeight(headerLow, 1.2f);
    football::MatchSimulationTestAccess::setBallHeight(headerHigh, 1.2f);
    football::MatchSimulationTestAccess::setAerialState(headerLow, 10, 0.9f);
    football::MatchSimulationTestAccess::setAerialState(headerHigh, 10, 0.9f);
    football::MatchSimulationTestAccess::setAerialAttributes(headerLow, 10, 60.0f, 25.0f, 55.0f);
    football::MatchSimulationTestAccess::setAerialAttributes(headerHigh, 10, 60.0f, 95.0f, 85.0f);
    assert(headerLow.command(10, football::PlayerAction::Header, -1, 1.0f, 1.0f));
    assert(headerHigh.command(10, football::PlayerAction::Header, -1, 1.0f, 1.0f));
    const float lowPower = std::sqrt(headerLow.ball().velocity.x * headerLow.ball().velocity.x +
                                     headerLow.ball().velocity.y * headerLow.ball().velocity.y);
    const float highPower = std::sqrt(headerHigh.ball().velocity.x * headerHigh.ball().velocity.x +
                                      headerHigh.ball().velocity.y * headerHigh.ball().velocity.y);
    assert(highPower > lowPower);
    assert(headerHigh.ball().velocity.x > 0.0f && headerHigh.ball().velocity.y > 0.0f);
    assert(headerHigh.ball().lastTouchTeam == 0);
    assert(headerHigh.players()[10].animation == football::PlayerAnimation::Header);

    football::MatchSimulation groundedHeader;
    groundedHeader.start();
    football::MatchSimulationTestAccess::setBall(groundedHeader, {0.5f, 0.0f}, -1);
    football::MatchSimulationTestAccess::setBallHeight(groundedHeader, 1.0f);
    assert(!groundedHeader.command(10, football::PlayerAction::Header, -1, 1.0f, 0.0f));

    football::MatchSimulation aerialFoul;
    aerialFoul.start();
    football::MatchSimulationTestAccess::setPlayer(aerialFoul, 10, {0.0f, 0.0f});
    football::MatchSimulationTestAccess::setPlayer(aerialFoul, 14, {0.72f, 0.0f});
    football::MatchSimulationTestAccess::setBall(aerialFoul, {0.35f, 0.0f}, -1);
    football::MatchSimulationTestAccess::setBallHeight(aerialFoul, 1.35f);
    football::MatchSimulationTestAccess::setAerialState(aerialFoul, 10, 0.8f);
    football::MatchSimulationTestAccess::setAerialState(aerialFoul, 14, 0.8f);
    football::MatchSimulationTestAccess::setAerialAttributes(aerialFoul, 10, 80.0f, 70.0f, 85.0f, 95.0f);
    assert(aerialFoul.command(10, football::PlayerAction::Sprint, -1, 1.0f, 0.0f));
    aerialFoul.update(0.05f);
    assert(aerialFoul.event() == football::MatchEvent::Foul);
    assert(aerialFoul.state() == football::MatchState::Paused);
    assert(aerialFoul.players()[10].animation == football::PlayerAnimation::AerialContact);
    assert(aerialFoul.players()[14].animation == football::PlayerAnimation::AerialFall);

    football::MatchSimulation automaticOffside;
    automaticOffside.start();
    football::MatchSimulationTestAccess::setPlayer(automaticOffside, 10, {8.0f, 0.0f});
    football::MatchSimulationTestAccess::setPlayer(automaticOffside, 9, {25.0f, 0.0f});
    for (int id = 11; id < 22; ++id)
        football::MatchSimulationTestAccess::setPlayer(automaticOffside, id, {id == 11 ? 35.0f : 20.0f, static_cast<float>(id - 16)});
    football::MatchSimulationTestAccess::setBall(automaticOffside, {8.0f, 0.0f}, 10);
    assert(automaticOffside.command(10, football::PlayerAction::Pass, 9, 1.0f, 0.0f));
    assert(automaticOffside.event() == football::MatchEvent::Offside);
    assert(automaticOffside.offsideLineVisible());
    assert(std::fabs(automaticOffside.offsideLine() - 20.0f) < 0.001f);
    const auto stoppedBall = automaticOffside.ball().position;
    automaticOffside.setControlRole(football::ControlRole::Referee);
    assert(automaticOffside.requestVarReview());
    assert(automaticOffside.resolveVar(false));
    assert(automaticOffside.state() == football::MatchState::Running);
    assert(automaticOffside.event() == football::MatchEvent::None);
    assert(!automaticOffside.offsideLineVisible());
    assert(std::fabs(automaticOffside.ball().position.x - stoppedBall.x) < 0.001f);

    football::MatchSimulation penaltyMatch;
    penaltyMatch.start();
    penaltyMatch.setControlRole(football::ControlRole::Referee);
    football::MatchSimulationTestAccess::setBall(penaltyMatch, {30.0f, 0.0f}, -1);
    assert(penaltyMatch.callFoul(12));
    assert(penaltyMatch.event() == football::MatchEvent::Penalty);
    assert(penaltyMatch.restartPlay());
    assert(penaltyMatch.ball().owner == 10);
    assert(std::fabs(penaltyMatch.ball().position.x - 29.5f) < 0.001f);

    football::MatchSimulation throwIn;
    throwIn.start();
    football::MatchSimulationTestAccess::setBall(throwIn, {4.0f, 54.9f}, -1);
    football::MatchSimulationTestAccess::setBallMotion(throwIn, {0.0f, 6.0f});
    throwIn.update(0.05f);
    assert(throwIn.state() == football::MatchState::Paused);
    assert(throwIn.event() == football::MatchEvent::ThrowIn);
    assert(throwIn.restartTeam() == 1);
    assert(throwIn.restartPlay());
    assert(throwIn.ball().owner >= 11 && throwIn.ball().owner < 22);

    football::MatchSimulation corner;
    corner.start();
    football::MatchSimulationTestAccess::setBall(corner, {35.5f, 4.5f}, -1);
    football::MatchSimulationTestAccess::setBallMotion(corner, {15.0f, 0.0f});
    football::MatchSimulationTestAccess::setLastTouchTeam(corner, 1);
    corner.update(0.05f);
    assert(corner.event() == football::MatchEvent::Corner);
    assert(corner.restartTeam() == 0);
    assert(corner.restartPlay());
    assert(corner.ball().position.x > 35.0f);
    assert(std::fabs(corner.ball().position.y) > 54.0f);

    football::MatchSimulation goalKick;
    goalKick.start();
    football::MatchSimulationTestAccess::setBall(goalKick, {35.5f, 8.0f}, -1);
    football::MatchSimulationTestAccess::setBallMotion(goalKick, {15.0f, 0.0f});
    goalKick.update(0.05f);
    assert(goalKick.event() == football::MatchEvent::GoalKick);
    assert(goalKick.restartTeam() == 1);
    assert(goalKick.restartPlay());
    assert(goalKick.ball().owner == 11);

    football::MatchSimulation advantage;
    advantage.start();
    advantage.setControlRole(football::ControlRole::Referee);
    assert(advantage.playAdvantage(12));
    assert(advantage.event() == football::MatchEvent::Advantage);
    football::MatchSimulationTestAccess::setBall(advantage, {1.0f, 0.0f}, 10);
    advantage.update(0.05f);
    assert(advantage.event() == football::MatchEvent::None);
    assert(advantage.state() == football::MatchState::Running);

    football::MatchSimulation goalVar;
    goalVar.start();
    football::MatchSimulationTestAccess::setBall(goalVar, {35.5f, 0.0f}, -1);
    football::MatchSimulationTestAccess::setBallMotion(goalVar, {20.0f, 0.0f});
    goalVar.update(0.05f);
    goalVar.setControlRole(football::ControlRole::Referee);
    assert(goalVar.requestVarReview());
    assert(goalVar.resolveVar(false));
    assert(goalVar.score(0) == 0);
    assert(goalVar.state() == football::MatchState::Paused);
    assert(goalVar.event() == football::MatchEvent::GoalKick);
    assert(goalVar.restartTeam() == 1);
    assert(goalVar.restartPlay());
    assert(goalVar.state() == football::MatchState::Running);

    football::MatchSimulation match;
    assert(match.playerCount() == 22);
    assert(match.activePlayerCount(0) == 11 && match.activePlayerCount(1) == 11);
    assert(match.state() == football::MatchState::Ready);
    match.update(1.0f);
    assert(match.matchSeconds() == 0.0f);
    match.start();
    assert(match.selectControlledPlayer(9));
    assert(!match.selectControlledPlayer(22));
    const auto controlledBefore = match.players()[9].position;
    assert(match.setPlayerMovement(1.0f, 0.0f));
    match.update(0.05f);
    assert(match.players()[9].position.x > controlledBefore.x);
    assert(match.selectControlledPlayer(10));
    assert(match.state() == football::MatchState::Running);
    assert(match.ball().owner == 10);
    match.update(0.25f);
    const float beforePause = match.matchSeconds();
    match.pause();
    const auto pausedPosition = match.players()[10].position;
    match.update(1.0f);
    assert(match.matchSeconds() == beforePause);
    assert(match.players()[10].position.x == pausedPosition.x);
    match.resume();
    match.update(0.1f);
    assert(match.matchSeconds() > beforePause);
    assert(match.setPlayerSentOff(2, true));
    assert(match.activePlayerCount(0) == 10);
    assert(!match.setPlayerSentOff(22, true));
    match.setSpectatorMode(true);
    assert(match.spectatorMode());
    assert(!match.command(10, football::PlayerAction::Shoot, -1, 1.0f, 0.0f));
    match.setSpectatorMode(false);
    assert(match.command(10, football::PlayerAction::Shoot, -1, 1.0f, 0.0f));
    match.setControlRole(football::ControlRole::Referee);
    assert(match.controlRole() == football::ControlRole::Referee);
    assert(match.issueCard(12, 1));
    assert(match.players()[12].yellowCards == 1);
    assert(match.players()[12].animation == football::PlayerAnimation::Protest);
    assert(match.issueCard(12, 1));
    assert(match.players()[12].sentOff);
    assert(match.issueCard(13, 2));
    assert(match.players()[13].sentOff);
    assert(match.players()[13].animation == football::PlayerAnimation::Dismissal);
    assert(!match.command(10, football::PlayerAction::Shoot, -1, 1.0f, 0.0f));
    match.setControlRole(football::ControlRole::Referee);
    assert(match.callFoul(14));
    assert(match.event() == football::MatchEvent::Foul);
    assert(match.restartTeam() == 0);
    assert(match.requestVarReview());
    assert(match.event() == football::MatchEvent::VarReview);
    assert(match.resolveVar(true));
    assert(match.event() == football::MatchEvent::Foul);
    assert(match.restartPlay());
    assert(match.state() == football::MatchState::Running);
    assert(match.callOffside(9));
    assert(match.event() == football::MatchEvent::Offside);
    assert(match.offsideLineVisible());
    assert(match.restartPlay());
    const float timeBeforeSlow = match.matchSeconds();
    match.setTimeScale(0.25f);
    match.update(0.05f);
    assert(std::fabs((match.matchSeconds() - timeBeforeSlow) - 0.0125f) < 0.0001f);
    match.finish();
    const float finishedAt = match.matchSeconds();
    match.update(0.2f);
    assert(match.matchSeconds() == finishedAt);
    match.reset();
    assert(match.state() == football::MatchState::Ready);
    assert(match.matchSeconds() == 0.0f);
    assert(match.score(0) == 0 && match.score(1) == 0);
    return 0;
}

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
};
}

int main() {
    football::MatchSimulation passMatch;
    passMatch.start();
    assert(passMatch.command(10, football::PlayerAction::Pass, -1, 1.0f, 0.0f));
    assert(passMatch.ball().owner == -1);
    assert(passMatch.ball().velocity.x > 0.0f);

    football::MatchSimulation keeperMatch;
    keeperMatch.start();
    assert(keeperMatch.selectControlledPlayer(0));
    assert(keeperMatch.setPlayerMovement(1.0f, 0.0f));
    for (int i = 0; i < 40; ++i) keeperMatch.update(0.05f);
    assert(keeperMatch.players()[0].position.x <= -26.0f);

    football::MatchSimulation keeperDive;
    keeperDive.start();
    football::MatchSimulationTestAccess::setBall(keeperDive, {-29.0f, 0.0f}, -1);
    assert(keeperDive.selectControlledPlayer(0));
    assert(keeperDive.command(0, football::PlayerAction::KeeperDive));
    assert(keeperDive.ball().owner == 0);
    assert(keeperDive.players()[0].animation == football::PlayerAnimation::KeeperDive);

    football::MatchSimulation keeperCatch;
    keeperCatch.start();
    football::MatchSimulationTestAccess::setBall(keeperCatch, {-30.0f, 0.0f}, -1);
    assert(keeperCatch.selectControlledPlayer(0));
    assert(keeperCatch.command(0, football::PlayerAction::KeeperCatch));
    assert(keeperCatch.ball().owner == 0);
    assert(keeperCatch.players()[0].animation == football::PlayerAnimation::KeeperCatch);

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
    assert(match.issueCard(12, 1));
    assert(match.players()[12].sentOff);
    assert(match.issueCard(13, 2));
    assert(match.players()[13].sentOff);
    assert(match.players()[13].animation == football::PlayerAnimation::Dismissal);
    assert(!match.command(10, football::PlayerAction::Shoot, -1, 1.0f, 0.0f));
    match.setControlRole(football::ControlRole::Referee);
    match.pause();
    assert(match.callFoul(12));
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

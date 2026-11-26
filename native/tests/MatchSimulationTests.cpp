#include "football/core/MatchSimulation.hpp"
#include <cassert>
#include <cmath>

int main() {
    football::MatchSimulation match;
    assert(match.playerCount() == 22);
    assert(match.activePlayerCount(0) == 11 && match.activePlayerCount(1) == 11);
    assert(match.state() == football::MatchState::Ready);
    match.update(1.0f);
    assert(match.matchSeconds() == 0.0f);
    match.start();
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

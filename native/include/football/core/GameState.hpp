#pragma once

#include <cstdint>

namespace football::core {
enum class MatchPhase : std::uint8_t {
    Menu,
    Kickoff,
    Playing,
    HalfTime,
    FullTime,
    Paused
};

struct GameState {
    MatchPhase phase{MatchPhase::Menu};
    std::uint32_t homeScore{0};
    std::uint32_t awayScore{0};
    float elapsedSeconds{0.0F};
};
}

#include "football/lobby/LobbyScene.hpp"

#include <cassert>
#include <cmath>

int main() {
    football::LobbyScene scene;
    scene.build();
    assert(!scene.playerNameVertices().empty());
    for(int player=0;player<22;++player) assert(scene.playerNameCount(player)>0);

    const auto& environment = scene.environmentVertices();
    const auto& players = scene.playerVertices();
    const auto& ball = scene.ballVertices();

    assert(environment.size() > 50000U);
    assert(players.size() > 1000U);
    assert(ball.size() > 100U);

    const auto finiteVertices = [](const std::vector<football::LobbyVertex>& vertices) {
        for (const auto& vertex : vertices) {
            for (float value : vertex.position) assert(std::isfinite(value));
            for (float value : vertex.normal) assert(std::isfinite(value));
            for (float value : vertex.color) assert(std::isfinite(value));
            for (float value : vertex.uv) assert(std::isfinite(value));
        }
    };
    finiteVertices(environment);
    finiteVertices(players);
    finiteVertices(ball);

    for (int formation = 0; formation < 4; ++formation) {
        scene.setFormation(formation);
        for (const auto& player : scene.playerPositions()) {
            assert(std::isfinite(player.x));
            assert(std::isfinite(player.z));
            assert(player.x >= -55.0F && player.x <= 55.0F);
            assert(player.z >= -36.0F && player.z <= 36.0F);
        }
    }
    return 0;
}

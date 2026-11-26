#pragma once

#include <array>
#include <cstddef>
#include <vector>

namespace football {

struct LobbyVertex {
    float position[3];
    float normal[3];
    float color[4];
    float uv[2];
    float boneIndices[4];
    float boneWeights[4];
    float material;
};

struct LobbyPlayerPosition {
    float x;
    float z;
    float yaw;
    float protest{0.0f};
    float pose{0.0f};
    float poseTime{0.0f};
    float speed{0.0f};
};

class LobbyScene final {
public:
    void build();
    void setFormation(int formation);

    const std::vector<LobbyVertex>& environmentVertices() const noexcept;
    const std::vector<LobbyVertex>& playerVertices() const noexcept;
    const std::vector<LobbyVertex>& ballVertices() const noexcept;
    const std::vector<LobbyVertex>& playerNameVertices() const noexcept;
    std::size_t playerNameStart(int playerId) const noexcept;
    std::size_t playerNameCount(int playerId) const noexcept;
    const std::array<LobbyPlayerPosition, 11>& playerPositions() const noexcept;

private:
    std::vector<LobbyVertex> environment_;
    std::vector<LobbyVertex> player_;
    std::vector<LobbyVertex> ball_;
    std::vector<LobbyVertex> playerNames_;
    std::array<std::size_t, 22> nameStarts_{};
    std::array<std::size_t, 22> nameCounts_{};
    std::array<LobbyPlayerPosition, 11> positions_{};
    void buildEnvironment();
    void buildPlayerModel();
    void buildPlayerNames();
    void buildBall();
    void updateFormation();
    int formation_{0};
};

} // namespace football
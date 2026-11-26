#pragma once

#include <array>
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
};

class LobbyScene final {
public:
    void build();
    void setFormation(int formation);

    const std::vector<LobbyVertex>& environmentVertices() const noexcept;
    const std::vector<LobbyVertex>& playerVertices() const noexcept;
    const std::array<LobbyPlayerPosition, 11>& playerPositions() const noexcept;

private:
    std::vector<LobbyVertex> environment_;
    std::vector<LobbyVertex> player_;
    std::array<LobbyPlayerPosition, 11> positions_{};
    void buildEnvironment();
    void buildPlayerModel();
    void updateFormation();
    int formation_{0};
};

} // namespace football
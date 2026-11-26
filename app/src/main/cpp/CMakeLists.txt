cmake_minimum_required(VERSION 3.22.1)
project(football_legends LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(FOOTBALL_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../native")

add_library(football_legends SHARED
    native-lib.cpp
    ${FOOTBALL_ROOT}/src/core/GameEngine.cpp
    ${FOOTBALL_ROOT}/src/core/MatchSimulation.cpp
    ${FOOTBALL_ROOT}/src/ai/TeamAI.cpp
    ${FOOTBALL_ROOT}/src/render/Renderer.cpp
    ${FOOTBALL_ROOT}/src/lobby/LobbyScene.cpp
)

target_include_directories(football_legends PRIVATE
    ${FOOTBALL_ROOT}/include
)

find_library(android-log log REQUIRED)
find_library(glesv3-lib GLESv3 REQUIRED)

target_link_libraries(football_legends
    ${android-log}
    ${glesv3-lib}
)
cmake_minimum_required(VERSION 3.22.1)
project(football_engine LANGUAGES CXX)

add_library(football_engine SHARED
    native-lib.cpp
    ../../../../native/src/core/GameState.cpp
    ../../../../native/src/render/Renderer.cpp
)

target_include_directories(football_engine PRIVATE
    ../../../../native/include
)

target_compile_features(football_engine PRIVATE cxx_std_17)
target_compile_options(football_engine PRIVATE -Wall -Wextra -Werror=return-type)
target_link_libraries(football_engine PRIVATE GLESv3 android log)

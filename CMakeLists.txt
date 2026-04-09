cmake_minimum_required(VERSION 3.16)
project(IronGrid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(irongrid
    src/main.cpp
    src/game/game.cpp
    src/units/unit.cpp
    src/units/knight.cpp
    src/units/archer.cpp
    src/units/lancer.cpp
    src/units/assassin.cpp
    src/grid/grid.cpp
    src/combat/combat.cpp
    src/ai/enemy_ai.cpp
)

target_include_directories(irongrid PRIVATE headers)

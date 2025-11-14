cmake_minimum_required(VERSION 3.10)

project(hand_learner_acquisition C CXX)

include(${PROJECT_SOURCE_DIR}/cmake/LoadSimpleBLE.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/LoadRaygui.cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

load_simple_ble()
load_raygui()

find_package(raylib REQUIRED)

add_subdirectory(app)

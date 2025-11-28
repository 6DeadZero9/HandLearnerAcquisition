cmake_minimum_required(VERSION 3.10)

project(hand_learner_acquisition C CXX)
set(CMAKE_CXX_STANDARD 20)

include(${PROJECT_SOURCE_DIR}/cmake/LoadSimpleBLE.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/LoadRaygui.cmake)

load_simple_ble()
load_raygui()

find_package(raylib REQUIRED)

add_subdirectory(app)

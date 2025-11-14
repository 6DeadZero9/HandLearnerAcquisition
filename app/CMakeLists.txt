find_package(Threads REQUIRED)
FetchContent_GetProperties(raygui)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

include_directories(${PROJECT_NAME} PUBLIC
    ${raygui_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/App
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/Control
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/DataControl
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    APPLICATION
)

target_link_libraries(${PROJECT_NAME} raylib simpleble)

cmake_minimum_required(VERSION 3.12)
project(Deer_Invaders)

set(CMAKE_CXX_STANDARD 17)

add_executable(Deer_Invaders
        Main.cpp
        Main.h
        State.cpp
        State.h
        Player.cpp
        Player.h
        Components.cpp
        Components.h
        Deers.cpp
        Deers.h)

target_link_libraries(Deer_Invaders PRIVATE SDL3 SDL3_image SDL3_ttf)

set(ASSETS_SRC_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_BINARY_DIR ${CMAKE_BINARY_DIR}/assets)
add_custom_target(
        assets ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ASSETS_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SRC_DIR} ${ASSETS_BINARY_DIR}
        COMMENT "Clearing and copying game assets into the binary directory."
)
add_dependencies(${PROJECT_NAME} assets)

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
        Personnages.cpp
        Personnages.h
        Entity.cpp
        Entity.h
        Database.cpp
        Database.h
        ObjectPool.cpp
        ObjectPool.h)

# Steamworks SDK
set(STEAM_SDK_PATH "${CMAKE_SOURCE_DIR}/libs/steamworks")
target_include_directories(Deer_Invaders PRIVATE ${STEAM_SDK_PATH}/public)
target_link_libraries(Deer_Invaders PRIVATE
        SDL3 SDL3_image SDL3_ttf sqlite3 SDL3_mixer
        ${STEAM_SDK_PATH}/redistributable_bin/win64/steam_api64.lib)

# Copy steam_api64.dll next to the .exe automatically
add_custom_command(TARGET Deer_Invaders POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${STEAM_SDK_PATH}/redistributable_bin/win64/steam_api64.dll"
        $<TARGET_FILE_DIR:Deer_Invaders>)

set(ASSETS_SRC_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_BINARY_DIR ${CMAKE_BINARY_DIR}/assets)
add_custom_target(
        assets ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ASSETS_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SRC_DIR} ${ASSETS_BINARY_DIR}
        COMMENT "Clearing and copying game assets into the binary directory."
)
add_dependencies(${PROJECT_NAME} assets)

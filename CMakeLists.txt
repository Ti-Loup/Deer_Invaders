cmake_minimum_required(VERSION 3.12)
project(Deer_Invaders)

set(CMAKE_CXX_STANDARD 17)

set(STEAM_SDK_PATH "${CMAKE_SOURCE_DIR}/libs/steamworks")
set(MIXER_PATH "${CMAKE_SOURCE_DIR}/libs/SDL3_mixer-3.2.0/x86_64-w64-mingw32")

add_executable(Deer_Invaders
        Main.cpp Main.h
        State.cpp State.h
        Player.cpp Player.h
        Components.cpp Components.h
        Personnages.cpp Personnages.h
        Entity.cpp Entity.h
        Database.cpp Database.h
        ObjectPool.cpp ObjectPool.h
)

target_include_directories(Deer_Invaders PRIVATE
        ${STEAM_SDK_PATH}/public
        ${MIXER_PATH}/include
)

find_library(MIXER_LIBRARY NAMES SDL3_mixer PATHS "${MIXER_PATH}/lib" NO_DEFAULT_PATH)

target_link_libraries(Deer_Invaders PRIVATE
        SDL3
        SDL3_image
        SDL3_ttf
        sqlite3
        ${MIXER_LIBRARY}
        "${STEAM_SDK_PATH}/redistributable_bin/win64/steam_api64.lib"
)

set(ASSETS_SRC_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_BINARY_DIR ${CMAKE_BINARY_DIR}/assets)

add_custom_target(assets ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ASSETS_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SRC_DIR} ${ASSETS_BINARY_DIR}
        COMMENT "Copie des assets vers le dossier de build..."
)
add_dependencies(Deer_Invaders assets)
set(BIN_DIR $<TARGET_FILE_DIR:Deer_Invaders>)

add_custom_command(TARGET Deer_Invaders POST_BUILD
        # Copie Steam API
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${STEAM_SDK_PATH}/redistributable_bin/win64/steam_api64.dll" "${BIN_DIR}"
        # Copie SDL3 Mixer
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MIXER_PATH}/bin/SDL3_mixer.dll" "${BIN_DIR}"
        COMMENT "Copie des DLL necessaires pour l'execution..."
)
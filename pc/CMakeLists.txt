cmake_minimum_required(VERSION 3.20)
project(mlpit_pc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 QUIET)
if(NOT SDL2_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
endif()

# Platform-layer sources (the SDL2 host code)
set(PC_SOURCES
    src/main.c
    src/platform_sdl.c
    src/nds_hw_stubs.c
    src/nds_vram.c
    src/arm_compat_host.c
    src/host_undefined_stubs.c
    src/game_thread.c
    src/nds_bg_render.c
    src/nds_rom.c
    src/asset_pack.c
    src/asset_extractor.c
)

# Decompiled ARM9 game code (compiled for host with HOST_PORT shim)
file(GLOB ARM9_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../arm9/src/*.c")

add_executable(mlpit_pc ${PC_SOURCES} ${ARM9_SOURCES})

target_include_directories(mlpit_pc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../arm9/include
    ${SDL2_INCLUDE_DIRS}
)

target_compile_definitions(mlpit_pc PRIVATE
    HOST_PORT=1
    SDL_MAIN_HANDLED=1
)

if(TARGET SDL2::SDL2)
    target_link_libraries(mlpit_pc PRIVATE SDL2::SDL2)
else()
    target_link_libraries(mlpit_pc PRIVATE ${SDL2_LIBRARIES})
    target_link_directories(mlpit_pc PRIVATE ${SDL2_LIBRARY_DIRS})
    target_compile_options(mlpit_pc PRIVATE ${SDL2_CFLAGS_OTHER})
endif()

# Suppress noisy warnings from decompiled code
target_compile_options(mlpit_pc PRIVATE
    -Wall
    -Wno-unused -Wno-unused-parameter -Wno-unused-variable
    -Wno-unused-function -Wno-unused-but-set-variable
    -Wno-pointer-sign -Wno-incompatible-pointer-types -Wno-int-conversion
    -Wno-implicit-function-declaration
    -Wno-builtin-declaration-mismatch
    -Wno-comment
    -fno-builtin
)

set_target_properties(mlpit_pc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

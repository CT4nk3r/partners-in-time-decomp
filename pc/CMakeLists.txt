cmake_minimum_required(VERSION 3.20)
project(mlpit_pc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 QUIET)
if(NOT SDL2_FOUND)
    # Fallback for MSYS2 / pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
endif()

# Source files
set(SOURCES
    src/main.c
    src/platform_sdl.c
    src/nds_hw_stubs.c
    src/nds_vram.c
)

add_executable(mlpit_pc ${SOURCES})

target_include_directories(mlpit_pc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

if(TARGET SDL2::SDL2)
    target_link_libraries(mlpit_pc PRIVATE SDL2::SDL2)
else()
    target_link_libraries(mlpit_pc PRIVATE ${SDL2_LIBRARIES})
    target_link_directories(mlpit_pc PRIVATE ${SDL2_LIBRARY_DIRS})
    target_compile_options(mlpit_pc PRIVATE ${SDL2_CFLAGS_OTHER})
endif()

# Skip SDL2main; we manage our own entry point and call SDL_SetMainReady()
target_compile_definitions(mlpit_pc PRIVATE SDL_MAIN_HANDLED)

target_compile_options(mlpit_pc PRIVATE
    -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable
)

# Output to top-level build/pc/ for consistency
set_target_properties(mlpit_pc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

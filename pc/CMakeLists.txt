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
    src/nds_boot_hook.c
    src/host_data_init.c
    src/host_display_data_init.c
    src/host_game_setup_overlay.c
    src/asset_viewer.c
    src/nds_arm9_ram.c
    src/host_scene_queue.c
    src/host_fnptr_resolver.c
    src/host_test_node.c
    src/host_factory_instantiate.c
    src/host_gxfifo_observer.c
    src/nds_hw_io.c
    src/host_oam_upload.c
    ${CMAKE_BINARY_DIR}/generated/nds_dat_init_generated.c
    ${CMAKE_BINARY_DIR}/generated/fnptr_table_generated.c
)

# Decompiled ARM9 game code (compiled for host with HOST_PORT shim)
file(GLOB ARM9_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../arm9/src/*.c")

add_executable(mlpit_pc ${PC_SOURCES} ${ARM9_SOURCES})

# Force-include the FUN_<addr> -> SDK/game-name alias header into every
# decompiled ARM9 translation unit (HOST_PORT only).  This rewires
# anonymous Ghidra calls (FUN_02005444, FUN_02046ffc, ...) onto the
# hand-named implementations in arm9/src/*.c so the no-op shadow stubs
# in pc/src/host_undefined_stubs.c become dead code.
set(SDK_ALIAS_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/sdk_symbol_aliases.h")
foreach(_arm9_src IN LISTS ARM9_SOURCES)
    set_source_files_properties(${_arm9_src} PROPERTIES
        COMPILE_OPTIONS "-include;${SDK_ALIAS_HEADER}")
endforeach()

# Copy runtime DLLs next to the exe so users can double-click without PATH setup.
# (MinGW + SDL2 produce dynamically linked binaries; missing DLLs = silent exit.)
if (WIN32)
    foreach (DLL SDL2.dll libgcc_s_seh-1.dll libwinpthread-1.dll libstdc++-6.dll)
        find_file(${DLL}_PATH NAMES ${DLL}
                  PATHS C:/msys64/mingw64/bin
                  NO_DEFAULT_PATH)
        if (${DLL}_PATH)
            add_custom_command(TARGET mlpit_pc POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${${DLL}_PATH}" "$<TARGET_FILE_DIR:mlpit_pc>/${DLL}")
        endif()
    endforeach()
endif()

target_include_directories(mlpit_pc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../arm9/include
    ${CMAKE_BINARY_DIR}/generated
    ${SDL2_INCLUDE_DIRS}
)

# ROM-data extraction: produce <build>/generated/host_data_init_generated.h
# at configure time from the user's own arm9.bin. The generated header
# contains copyrighted ROM bytes and lives under build/ which is gitignored
# (never commit the output, only the script).
set(_extract_script "${CMAKE_CURRENT_SOURCE_DIR}/../tools/scripts/extract_data_inits.py")
set(_arm9_bin "${CMAKE_CURRENT_SOURCE_DIR}/../extracted/arm9.bin")
set(_data_init_header "${CMAKE_BINARY_DIR}/generated/host_data_init_generated.h")
find_package(Python3 COMPONENTS Interpreter QUIET)
if(NOT Python3_Interpreter_FOUND)
    set(Python3_EXECUTABLE python)
endif()
if(EXISTS ${_arm9_bin})
    execute_process(
        COMMAND ${Python3_EXECUTABLE} ${_extract_script} ${_arm9_bin} ${_data_init_header}
        RESULT_VARIABLE _extract_rc
    )
    if(NOT _extract_rc EQUAL 0)
        message(FATAL_ERROR "extract_data_inits.py failed (rc=${_extract_rc})")
    endif()
else()
    message(WARNING "extracted/arm9.bin not found - run tools/scripts/extract_rom.py first; building with stale generated header (if any).")
endif()
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/host_data_init.c
    PROPERTIES OBJECT_DEPENDS ${_data_init_header})

# DAT_<addr> wholesale .data initializer: parse host_undefined_stubs.c
# for every `DAT_<addr>` declaration, emit (1) a header with extern
# decls + a (nds_addr, &symbol, size) table and (2) a C source that
# memcpys bytes from the VirtualAlloc'd arm9.bin shadow into each
# host-side global.  No ROM bytes enter the build tree - the table is
# just addresses.
set(_dat_gen_script "${CMAKE_CURRENT_SOURCE_DIR}/../tools/scripts/gen_dat_init_from_stubs.py")
set(_dat_table_h    "${CMAKE_BINARY_DIR}/generated/dat_init_table.h")
set(_dat_init_src   "${CMAKE_BINARY_DIR}/generated/nds_dat_init_generated.c")
execute_process(
    COMMAND ${Python3_EXECUTABLE} ${_dat_gen_script}
            ${CMAKE_CURRENT_SOURCE_DIR}/src/host_undefined_stubs.c
            ${_dat_table_h}
            ${_dat_init_src}
    RESULT_VARIABLE _dat_rc
)
if(NOT _dat_rc EQUAL 0)
    message(FATAL_ERROR "gen_dat_init_from_stubs.py failed (rc=${_dat_rc})")
endif()
set_source_files_properties(${_dat_init_src}
    PROPERTIES OBJECT_DEPENDS "${_dat_table_h};${_dat_gen_script};${CMAKE_CURRENT_SOURCE_DIR}/src/host_undefined_stubs.c")

# fnptr trampoline table: enumerates every FUN_<addr> defined in
# arm9/src/*.c and registers the host symbol address under that NDS
# code address, so the scene-queue processor can dispatch ARM-domain
# vtable entries to their decompiled C bodies.
set(_fnptr_gen_script "${CMAKE_CURRENT_SOURCE_DIR}/../tools/scripts/gen_fnptr_table.py")
set(_fnptr_gen_src    "${CMAKE_BINARY_DIR}/generated/fnptr_table_generated.c")
execute_process(
    COMMAND ${Python3_EXECUTABLE} ${_fnptr_gen_script}
            ${CMAKE_CURRENT_SOURCE_DIR}/../arm9/src
            ${_fnptr_gen_src}
    RESULT_VARIABLE _fnptr_rc
)
if(NOT _fnptr_rc EQUAL 0)
    message(FATAL_ERROR "gen_fnptr_table.py failed (rc=${_fnptr_rc})")
endif()
set_source_files_properties(${_fnptr_gen_src}
    PROPERTIES OBJECT_DEPENDS "${_fnptr_gen_script}"
               COMPILE_OPTIONS "-include;${SDK_ALIAS_HEADER}")

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

if(WIN32)
    target_link_libraries(mlpit_pc PRIVATE dbghelp)
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

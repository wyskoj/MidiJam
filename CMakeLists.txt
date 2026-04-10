cmake_minimum_required(VERSION 3.10)
project(MidiJam)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

set(CMAKE_VERBOSE_MAKEFILE ON)

# ---------------------------------------------------------------------------
# C++ standard — VS2003 does not support the /std:c++11 flag
# ---------------------------------------------------------------------------
if(NOT (MSVC AND MSVC_VERSION LESS 1400))
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# ---------------------------------------------------------------------------
# Linker flags — fixed base address + map file for midijamgui.py symbol lookup
# ---------------------------------------------------------------------------
if(NOT (MSVC AND MSVC_VERSION LESS 1400))
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /BASE:0x400000 /MAP /OPT:NOREF")
else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MAP")
endif()

# ---------------------------------------------------------------------------
# Strip unwanted flags injected by CMake defaults (applies to all MSVC versions)
# ---------------------------------------------------------------------------
if(MSVC)
    foreach(flag_var
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_MINSIZEREL)
        # Remove runtime checks — injected as /RTC1 in Debug by default
        string(REGEX REPLACE "/RTC[^ ]*" "" ${flag_var} "${${flag_var}}")
    endforeach()

    # Remove CMake's default /O2 and /Ob2 from Release
    string(REPLACE "/O2"  "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "/Ob2" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
endif()

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------
add_executable(MidiJam WIN32
        main.cpp
        globals.cpp
        macros.h
        audio/DirectMusicSegmentPlayer.cpp
        audio/DirectMusicSystem.cpp
        audio/DlsCollectionWrapper.cpp
        audio/DlsCollectionWrapper.h
        audio/MidiJamTool.cpp
        audio/MidiJamTool.h
        audio/initialize.cpp
        audio/initialize.h
        audio/playback.cpp
        audio/playback.h
        instruments/Accordion.cpp
        instruments/Accordion.h
        instruments/Bass.cpp
        instruments/Bass.h
        instruments/Harp.cpp
        instruments/Harp.h
        instruments/Piano.cpp
        instruments/Piano.h
        instruments/StageHorn.cpp
        instruments/StageHorn.h
        instruments/instrument.h
        instruments/instrument_ids.h
        instruments/keys.cpp
        instruments/keys.h
        math/AffineTransform4f.h
        math/AffineTransform4f.cpp
        math/TransformVec4.cpp
        math/Quaternion.cpp
        math/scalar.cpp
        model/Ms3dBundle.cpp
        model/Ms3dBundle.h
        model/Ms3dJoint.cpp
        model/Ms3dJoint.h
        model/animation/AnimationController.cpp
        model/animation/AnimationController.h
        model/ms3d_format.h
        render/gl.cpp
        render/gl.h
        render/text.cpp
        render/text.h
        render/texture.cpp
        render/texture.h
        render/window.cpp
        render/window.h
        scene/camera.h
        scene/fadeout.cpp
        scene/fadeout.h
        scene/scene.cpp
        scene/scene.h
        scene/update.cpp
        scene/update.h
        util/guid.cpp
        util/string.cpp
        compat/vs2003.h
        instruments/StageString.cpp
        instruments/StageString.h
        instruments/Xylophone.cpp
        instruments/Xylophone.h
        instruments/Trombone.cpp
        instruments/Trombone.h
        instruments/Guitar.cpp
        instruments/Guitar.h
        scene/camera.cpp
        instruments/Agogos.cpp
        instruments/Agogos.h
        instruments/Woodblocks.cpp
        instruments/Woodblocks.h
)

# ---------------------------------------------------------------------------
# Per-target compile options — version-gated
# ---------------------------------------------------------------------------
if(MSVC)
    if(MSVC_VERSION LESS 1400)
        # -----------------------------------------------------------------
        # VS2003 (MSVC 13.10) — the target compiler
        # /arch:IA32, /GS-, /fp:precise do not exist in VS2003.
        # VS2003 defaults to plain x86, no SSE, no security cookies,
        # and precise FP — so no flags needed for those.
        # -----------------------------------------------------------------
        target_compile_options(MidiJam PRIVATE
                /W3             # Warning level
                /Oy-            # Keep frame pointers
                /Od             # No optimization
                /Ob0            # No inlining
                /FI${CMAKE_SOURCE_DIR}/compat/vs2003.h  # Force-include compat shims
        )
    else()
        # -----------------------------------------------------------------
        # Modern MSVC — approximate original compiler behavior
        # -----------------------------------------------------------------
        target_compile_options(MidiJam PRIVATE
                /arch:IA32      # No SSE — original was plain x86
                /W3             # Warning level
                /GS-            # No security cookies
                /Oy-            # Keep frame pointers
                /fp:precise     # Floating point model

                # No optimization in all configs — matches original instruction patterns
                $<$<CONFIG:Release>:/Od>
                $<$<CONFIG:Release>:/Ob0>
                $<$<CONFIG:Debug>:/Od>
                $<$<CONFIG:Debug>:/Ob0>
        )
    endif()
endif()

# ---------------------------------------------------------------------------
# Libraries
# ---------------------------------------------------------------------------
target_link_libraries(MidiJam PRIVATE
        opengl32.lib
        glu32.lib
        winmm.lib
)

# ---------------------------------------------------------------------------
# Include dirs
# ---------------------------------------------------------------------------
target_include_directories(MidiJam PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/sdk
)

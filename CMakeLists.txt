add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

cmake_minimum_required(VERSION 3.20)
project(MidiJam)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /BASE:0x400000 /MAP")

if(MSVC)
    add_compile_options(
            /arch:IA32
            /W3
            /GS-
    )
    string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
endif()

add_executable(MidiJam WIN32
        main.cpp
        globals.cpp
        macros.h
        audio/DirectMusicSegmentWrapper.cpp
        audio/DirectMusicSegmentWrapper.h
        audio/DirectMusicSystem.cpp
        audio/DlsCollectionWrapper.cpp
        audio/DlsCollectionWrapper.h
        audio/MidiJamTool.cpp
        audio/MidiJamTool.h
        audio/initialize.cpp
        audio/initialize.h
        audio/playback.cpp
        audio/playback.h
        instruments/Harp.h
        instruments/Piano.cpp
        instruments/Piano.h
        instruments/instrument.h
        instruments/instrument_ids.h
        instruments/keys.cpp
        instruments/keys.h
        math/MatrixMath.cpp
        math/Quaternion.cpp
        math/QuaternionD.cpp
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
        util/time.cpp
        util/time.h
        instruments/Bass.cpp
        instruments/Bass.h
)

target_link_libraries(MidiJam PRIVATE opengl32.lib glu32.lib)

if(MSVC)
    target_compile_options(MidiJam PRIVATE
            $<$<CONFIG:Release>:/O2>
            $<$<CONFIG:Release>:/Oi>
            $<$<CONFIG:Release>:/Ot>
    )
endif()

target_include_directories(MidiJam PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/sdk
)

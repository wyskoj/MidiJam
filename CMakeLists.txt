cmake_minimum_required(VERSION 4.0)
project(MidiJam)

set(CMAKE_CXX_STANDARD 98)

add_executable(MidiJam src/main.cpp
        src/Ms3dBundle.cpp
        src/Ms3dBundle.h
        src/AnimationController.cpp
        src/AnimationController.h
        src/Vector3.h
        src/textures.cpp
        src/textures.h
        src/MatrixMath.cpp
        src/MatrixMath.h
        src/Quaternion.cpp
        src/Quaternion.h
        src/ObjectConstructor.h
        src/DirectMusicSystem.cpp
        src/DirectMusicSystem.h
        src/DirectMusicSegmentWrapper.cpp
        src/DirectMusicSegmentWrapper.h
        src/MidiJam.h
        src/MidiJam.cpp
        src/globals.h
        src/globals.cpp
        src/camera.h
        src/fade.h
        src/fade.cpp
        src/text.cpp
        src/text.h
        src/IMidiJamTool.cpp
        src/IMidiJamTool.h
        src/fonts.cpp
        src/fonts.h
        src/hwf.cpp
        src/hwf.h
        src/midi.cpp
        src/midi.h
        src/instrument/Piano.cpp
        src/instrument/Piano.h
        src/instrument/instrument.h
        src/glhelp.h
        src/instrument/instrument.cpp
        src/camera.cpp
        src/input.cpp
        src/input.h
)

target_include_directories(MidiJam PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_directories(MidiJam PRIVATE "$ENV{DXSDK_DIR}/Lib/x86")
target_link_libraries(MidiJam PRIVATE opengl32 glu32 winmm ole32 dxguid)

add_custom_command(TARGET MidiJam POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/res/MidiJam.HWF"
        "$<TARGET_FILE_DIR:MidiJam>/MidiJam.HWF"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/res/HWF.cfg"
        "$<TARGET_FILE_DIR:MidiJam>/HWF.cfg"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/res/Config.exe"
        "$<TARGET_FILE_DIR:MidiJam>/Config.exe"
)
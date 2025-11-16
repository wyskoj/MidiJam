cmake_minimum_required(VERSION 4.0)
project(MidiJam)

set(CMAKE_CXX_STANDARD 98)

add_executable(MidiJam src/main.cpp)

target_include_directories(MidiJam PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_directories(MidiJam PRIVATE "$ENV{DXSDK_DIR}/Lib/x86")
target_link_libraries(MidiJam PRIVATE opengl32 glu32 winmm ole32 dxguid)
@echo off
setlocal

:: Set up VS2003 environment
call "C:\Program Files (x86)\Microsoft Visual Studio .NET 2003\Vc7\bin\vcvars32.bat"

:: Create build directory
if not exist cmake-build-vs2003 mkdir cmake-build-vs2003
cd cmake-build-vs2003

:: Configure with NMake generator — calls cl.exe directly
cmake .. ^
    -G "NMake Makefiles" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_C_COMPILER="C:/Program Files (x86)/Microsoft Visual Studio .NET 2003/Vc7/bin/cl.exe" ^
    -DCMAKE_CXX_COMPILER="C:/Program Files (x86)/Microsoft Visual Studio .NET 2003/Vc7/bin/cl.exe" ^
    -DCMAKE_LINKER="C:/Program Files (x86)/Microsoft Visual Studio .NET 2003/Vc7/bin/link.exe" ^
    -DCMAKE_MAKE_PROGRAM="C:/Program Files (x86)/Microsoft Visual Studio .NET 2003/Vc7/bin/nmake.exe"

:: Build
nmake

cd ..
echo.
echo Build complete. Binary at:
echo cmake-build-vs2003\MidiJam.exe
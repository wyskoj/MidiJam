# MidiJam Decompilation

This is a work-in-progress decompilation of MidiJam, a DirectMusic-powered 3D instrument visualizer last developed around 2007. This project aims to be as accurate as possible—that is, matching the recompiled instructions to the original machine code as much as possible. The goal is to provide a workable codebase that can be understood, modified, and built upon.

> **Note:** This repository is for decompilation only and its code is true to the original release. It targets 32-bit Windows and will not compile for other platforms without significant porting work.

## Building

This project uses [CMake](https://cmake.org/). For the most accurate results, **Microsoft Visual C++ .NET 2003 (MSVC 13.10)** — the compiler used to build the original executable — is recommended. The `build_vs2003.bat` script automates the VS2003 build. Modern MSVC can also be used for development and verification purposes.

### Prerequisites

- **Microsoft Visual C++ .NET 2003** for highest-accuracy builds, or any modern MSVC toolchain for development builds.
- [CMake](https://cmake.org/) 3.10 or newer.

### Building with VS2003 (recommended for accuracy)

1. Open a Command Prompt.
2. Run the VS2003 environment setup script:
   ```
   "C:\Program Files (x86)\Microsoft Visual Studio .NET 2003\Vc7\bin\vcvars32.bat"
   ```
3. From the repository root, run:
   ```
   build_vs2003.bat
   ```
4. The compiled executable will be placed at `cmake-build-vs2003\MidiJam.exe`.

### Building with modern MSVC

1. Open a Developer Command Prompt for your Visual Studio installation.
2. Create a build directory and configure:
   ```
   mkdir cmake-build
   cd cmake-build
   cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
   ```
3. Build:
   ```
   nmake
   ```

> **Note:** The CMake configuration disables runtime checks (`/RTC1`), inlining (`/Ob0`), and optimization (`/Od`) to match the original compiler output patterns. When building with modern MSVC, `/arch:IA32` and `/GS-` are additionally applied to suppress SSE and security cookie instrumentation.

## Usage

Place the compiled `MidiJam.exe` in the original MidiJam installation directory alongside `MidiJam.HWF` (the asset archive) and `config.exe`. Run it with a MIDI file path as the command-line argument:

```
MidiJam.exe "path\to\song.mid"
```

If no argument is provided, the application attempts to load `rocky_1.mid` from the working directory. On first run, if `HWF.cfg` is missing, `config.exe` is launched automatically to configure display and audio settings.

## Additional Information

### Target binary

This decompilation targets the latest release of MidiJam (1.12) compiled with MSVC 13.10 (Visual Studio .NET 2003), loaded at base address `0x400000`. Function addresses in source comments (e.g. `// FUNCTION: MIDIJAM 0x401000`) refer to virtual addresses in the original executable.

Match quality annotations are included in each transcribed function:

- `// MATCH: EXACT` — recompiled output is instruction-identical to the original.
- `// MATCH: APPROXIMATE` — output is functionally equivalent with minor codegen differences (noted inline).
- No annotation — transcription is complete but match quality has not been verified.

### HWF archive format

`MidiJam.HWF` is a custom flat archive format containing all model, texture, and font assets. The appendix (a directory of entry names and sizes) is stored at the end of the file. See `render/texture.cpp` (`ReadHwfAppendix`) for the parsing implementation.
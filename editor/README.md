# Jankspire Editor

The editor is the face of the engine: the main tool for working with the
engine library. Right now it's a hello world that builds through CMake.

## Prerequisites

- macOS 27 SDK (Xcode, with its license accepted: `sudo xcodebuild -license`)
- CMake 3.28+ (`brew install cmake`)

## Build and run

All commands run from the **repo root**, not from `editor/`:

```bash
# configure: reads CMakeLists.txt, generates build files into build/
# (only needed the first time, or after changing a CMakeLists.txt)
cmake -S . -B build

# build: compiles and links whatever changed
cmake --build build

# run:
./build/editor/jankspire-editor

# or simply...
cmake --build build && ./build/editor/jankspire-editor

```

Build output lives in `build/` and is gitignored. If the build ever gets into a
weird state, `rm -rf build` and configure again.

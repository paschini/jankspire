cmake_minimum_required(VERSION 3.28)
# OBJCXX: Objective-C++ (.mm), needed by Dear ImGui's macOS/Metal backends
project(Jankspire LANGUAGES CXX OBJCXX)

# Writes build/compile_commands.json so clangd in nvim knows
# the include paths and flags for every file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(third_party)
add_subdirectory(editor)

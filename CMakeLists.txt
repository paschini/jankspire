cmake_minimum_required(VERSION 3.28)
project(Jankspire LANGUAGES CXX)

# Writes build/compile_commands.json so clangd in nvim knows
# the include paths and flags for every file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(editor)

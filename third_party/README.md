# third_party

## Vendored (committed)

| Folder | What | Version | Source | License |
|---|---|---|---|---|
| `metal-cpp/` | Apple's header-only C++ interface for Metal, Foundation, QuartzCore | `metal-cpp_macOS27_iOS27` (macOS 27 SDK) | https://developer.apple.com/metal/cpp | Apache 2.0 |
| `metal-cpp-extensions/` | C++ wrappers for the bits of AppKit and MetalKit that Apple's samples need (`NSApplication`, `NSWindow`, `NSMenu`, `MTKView`) | from the "Learn Metal with C++" sample code (headers dated 2022) | https://developer.apple.com/metal/cpp | Apache 2.0 |
| `imgui/` | Dear ImGui (docking branch): immediate-mode UI for the editor, plus its Metal and macOS backends | `v1.92.9b-docking` (commit `b48d1af`) | https://github.com/ocornut/imgui | MIT |

`imgui/` is a trimmed copy: the core files, `backends/imgui_impl_metal.*` and
`backends/imgui_impl_osx.*`, `misc/cpp/` (std::string helpers) and the license.
Docs, examples and other platforms' backends are left out.

The targets these folders build (`metal-cpp`, `imgui`) are defined in
`CMakeLists.txt` here; consumers just `target_link_libraries` them.

Upgrading: download the new version, replace the folder contents (keep the
folder name), update the version column above, and commit it as its own commit.

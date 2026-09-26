# Build targets for vendored third-party code. Everything else links these by
# name, e.g. target_link_libraries(jankspire-editor PRIVATE imgui), instead of
# repeating include paths and framework flags.

# --- metal-cpp -----------------------------------------------------------------
# Header-only, so an INTERFACE library: nothing gets compiled, the target just
# carries its include paths and frameworks to whoever links it. SYSTEM keeps
# our strict warning flags off Apple's headers.
add_library(metal-cpp INTERFACE)
# metal-cpp's headers use C++17 features, so anything including them needs at least that
target_compile_features(metal-cpp INTERFACE cxx_std_17)
target_include_directories(metal-cpp SYSTEM INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp-extensions
)
target_link_libraries(metal-cpp INTERFACE
  "-framework Foundation"
  "-framework QuartzCore"
  "-framework Metal"
  "-framework MetalKit"
  "-framework AppKit"
)

# --- Dear ImGui (docking branch) + Metal/macOS backends --------------------------
add_library(imgui STATIC
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui_demo.cpp
  imgui/misc/cpp/imgui_stdlib.cpp
  imgui/backends/imgui_impl_metal.mm
  imgui/backends/imgui_impl_osx.mm
)
target_include_directories(imgui SYSTEM PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp
)
# Switch the backends' public API to metal-cpp types (MTL::Device*, etc.).
# PUBLIC, because the backend .mm files and our code must agree on it.
target_compile_definitions(imgui PUBLIC
  IMGUI_IMPL_METAL_CPP
  IMGUI_IMPL_METAL_CPP_EXTENSIONS
)
# Both backends are Objective-C++. The macOS one is written for ARC (automatic
# reference counting). The Metal one, in metal-cpp mode, releases objects by
# hand, which ARC forbids, so it compiles without ARC.
set_source_files_properties(
  imgui/backends/imgui_impl_osx.mm
  PROPERTIES COMPILE_OPTIONS "-fobjc-arc"
)
# Upstream's warnings (macOS 27 deprecations, ARC-only casts in the non-ARC
# Metal backend) are theirs to fix; we don't patch vendored code.
target_compile_options(imgui PRIVATE -w)
target_link_libraries(imgui PUBLIC
  metal-cpp
  "-framework Carbon"
  "-framework GameController"
)

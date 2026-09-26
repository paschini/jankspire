add_executable(jankspire-editor
  main.cpp
  EditorApp.cpp
  EditorTheme.cpp
  EditorUi.cpp
  Renderer.cpp
)

target_compile_features(jankspire-editor PRIVATE cxx_std_20)
target_compile_options(jankspire-editor PRIVATE -Wall -Wextra -Wpedantic)

# Targets defined in third_party/CMakeLists.txt; they bring their own
# include paths and frameworks along
target_link_libraries(jankspire-editor PRIVATE
  metal-cpp
  imgui
)

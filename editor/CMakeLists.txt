add_executable(jankspire-editor
    main.cpp
    EditorApp.cpp
    Renderer.cpp
)

target_compile_features(jankspire-editor PRIVATE cxx_std_20)
target_compile_options(jankspire-editor PRIVATE -Wall -Wextra -Wpedantic)

# metal-cpp headers. SYSTEM marks them as third-party, so our strict warning
# flags apply to our code only, not to Apple's headers.
target_include_directories(jankspire-editor SYSTEM PRIVATE
    ${PROJECT_SOURCE_DIR}/third_party/metal-cpp
    ${PROJECT_SOURCE_DIR}/third_party/metal-cpp-extensions
)

# The macOS frameworks that metal-cpp's calls land in at link time
target_link_libraries(jankspire-editor PRIVATE
    "-framework Foundation"
    "-framework QuartzCore"
    "-framework Metal"
    "-framework MetalKit"
    "-framework AppKit"
)

#pragma once

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

// Owns the Dear ImGui context and draws the editor's panels each frame:
// a toolbar, the inspector, the scene viewport and a status bar.
class EditorUi
{
public:
  EditorUi(MTL::Device* device, MTK::View* view);
  ~EditorUi();

  // One ImGui context per process; copying or moving this would double-free it
  EditorUi(const EditorUi&) = delete;
  EditorUi& operator=(const EditorUi&) = delete;
  EditorUi(EditorUi&&) = delete;
  EditorUi& operator=(EditorUi&&) = delete;

  // Builds this frame's UI. Call before creating the render pass encoder.
  void BeginFrame(MTK::View* view, MTL::RenderPassDescriptor* passDescriptor);

  // Encodes the UI's draw commands into the current render pass.
  static void Render(MTL::CommandBuffer* commandBuffer, MTL::RenderCommandEncoder* encoder);

private:
  bool m_isLayoutBuilt = false;
};

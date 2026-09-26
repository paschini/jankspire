#pragma once

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

class EditorUi;

// MTKView calls drawInMTKView once per frame (60 fps by default).
class Renderer : public MTK::ViewDelegate
{
public:
  // `ui` is borrowed, not owned: it must outlive the Renderer
  Renderer(MTL::Device* device, EditorUi* ui);

  void drawInMTKView(MTK::View* view) override;

private:
  NS::SharedPtr<MTL::CommandQueue> m_commandQueue;
  EditorUi* m_ui;
};

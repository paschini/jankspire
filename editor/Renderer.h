
#pragma once

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

// MTKView calls drawInMTKView once per frame (60 fps by default).
class Renderer : public MTK::ViewDelegate
{
public:
  explicit Renderer(MTL::Device* device);

  void drawInMTKView(MTK::View* view) override;

private:
  NS::SharedPtr<MTL::CommandQueue> m_commandQueue;
};

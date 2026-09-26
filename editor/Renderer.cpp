#include "Renderer.h"

#include "EditorUi.h"

Renderer::Renderer(MTL::Device* device, EditorUi* ui)
    : m_commandQueue(NS::TransferPtr(device->newCommandQueue())), m_ui(ui)
{
}

void Renderer::drawInMTKView(MTK::View* view)
{
  // Metal hands back "autoreleased" objects (command buffer, pass descriptor)
  // every frame; this pool frees them at the end of the frame.
  NS::SharedPtr<NS::AutoreleasePool> pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

  // The pass descriptor says "clear to the view's clearColor" when the pass starts.
  // It's null when there's nothing to draw into (e.g. the window is minimized).
  MTL::RenderPassDescriptor* passDescriptor = view->currentRenderPassDescriptor();
  if (passDescriptor == nullptr)
  {
    return;
  }

  MTL::CommandBuffer* commandBuffer = m_commandQueue->commandBuffer();

  m_ui->BeginFrame(view, passDescriptor);

  MTL::RenderCommandEncoder* encoder = commandBuffer->renderCommandEncoder(passDescriptor);
  EditorUi::Render(commandBuffer, encoder);
  encoder->endEncoding();

  commandBuffer->presentDrawable(view->currentDrawable());
  commandBuffer->commit();
}

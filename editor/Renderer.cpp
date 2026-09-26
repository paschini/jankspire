#include "Renderer.h"

Renderer::Renderer(MTL::Device* device)
    : m_commandQueue(NS::TransferPtr(device->newCommandQueue()))
{
}

void Renderer::drawInMTKView(MTK::View* view)
{
    // Metal hands back "autoreleased" objects (command buffer, pass descriptor)
    // every frame; this pool frees them at the end of the frame.
    NS::SharedPtr<NS::AutoreleasePool> pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

    MTL::CommandBuffer* commandBuffer = m_commandQueue->commandBuffer();

    // The pass descriptor says "clear to the view's clearColor" when the pass starts
    MTL::RenderPassDescriptor* passDescriptor = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* encoder = commandBuffer->renderCommandEncoder(passDescriptor);
    encoder->endEncoding();

    commandBuffer->presentDrawable(view->currentDrawable());
    commandBuffer->commit();
}

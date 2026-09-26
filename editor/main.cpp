// The one file that compiles metal-cpp's function bodies. Every other file
// includes these headers *without* the defines.
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "EditorApp.h"

int main()
{
    NS::SharedPtr<NS::AutoreleasePool> pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

    EditorApp editorApp;

    NS::Application* app = NS::Application::sharedApplication();
    app->setDelegate(&editorApp);
    app->run();
}

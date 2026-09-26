#pragma once

#include <memory>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "EditorUi.h"
#include "Renderer.h"

// Owns the editor's window and wires AppKit's lifecycle callbacks to our code.
// AppKit calls these methods; their camelCase names are Apple's, not ours.
class EditorApp : public NS::ApplicationDelegate
{
public:
  void applicationWillFinishLaunching(NS::Notification* notification) override;
  void applicationDidFinishLaunching(NS::Notification* notification) override;
  bool applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender) override;

private:
  static NS::Menu* CreateMenuBar();

  NS::SharedPtr<MTL::Device> m_device;
  NS::SharedPtr<NS::Window> m_window;
  NS::SharedPtr<MTK::View> m_view;
  // Declared before m_renderer so it's destroyed after it: the renderer borrows it
  std::unique_ptr<EditorUi> m_ui;
  std::unique_ptr<Renderer> m_renderer;
};

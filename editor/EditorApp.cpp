#include "EditorApp.h"

void EditorApp::applicationWillFinishLaunching(NS::Notification* notification)
{
  auto* app = static_cast<NS::Application*>(notification->object());
  app->setMainMenu(CreateMenuBar());
  app->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void EditorApp::applicationDidFinishLaunching(NS::Notification* notification)
{
  constexpr CGRect Frame = {.origin = {.x = 100.0, .y = 100.0}, .size = {.width = 1280.0, .height = 720.0}};

  constexpr NS::WindowStyleMask Style = NS::WindowStyleMaskTitled | NS::WindowStyleMaskClosable |
                                        NS::WindowStyleMaskMiniaturizable | NS::WindowStyleMaskResizable;
  m_window = NS::TransferPtr(NS::Window::alloc()->init(Frame, Style, NS::BackingStoreBuffered, false));

  m_device = NS::TransferPtr(MTL::CreateSystemDefaultDevice());

  m_view = NS::TransferPtr(MTK::View::alloc()->init(Frame, m_device.get()));
  m_view->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
  m_view->setClearColor(MTL::ClearColor::Make(0.16, 0.16, 0.21, 1.0));

  m_renderer = std::make_unique<Renderer>(m_device.get());
  m_view->setDelegate(m_renderer.get());

  m_window->setContentView(m_view.get());
  m_window->setTitle(NS::String::string("Jankspire Editor", NS::StringEncoding::UTF8StringEncoding));
  m_window->makeKeyAndOrderFront(nullptr);

  auto* app = static_cast<NS::Application*>(notification->object());
  app->activateIgnoringOtherApps(true);
}

bool EditorApp::applicationShouldTerminateAfterLastWindowClosed(NS::Application* /*sender*/)
{
  return true;
}

NS::Menu* EditorApp::CreateMenuBar()
{
  using NS::StringEncoding::UTF8StringEncoding;

  NS::Menu* mainMenu = NS::Menu::alloc()->init();

  // The first menu is always the "app menu" (the bold one next to the Apple logo)
  NS::MenuItem* appMenuItem = NS::MenuItem::alloc()->init();
  NS::Menu* appMenu = NS::Menu::alloc()->init(NS::String::string("Jankspire", UTF8StringEncoding));

  SEL quitAction = NS::MenuItem::registerActionCallback(
    "appQuit", [](void*, SEL, const NS::Object* sender) { NS::Application::sharedApplication()->terminate(sender); });

  NS::MenuItem* quitItem = appMenu->addItem(
    NS::String::string("Quit Jankspire Editor", UTF8StringEncoding), quitAction,
    NS::String::string("q", UTF8StringEncoding));
  quitItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);

  appMenuItem->setSubmenu(appMenu);
  mainMenu->addItem(appMenuItem);

  appMenuItem->release();
  appMenu->release();

  return mainMenu->autorelease();
}

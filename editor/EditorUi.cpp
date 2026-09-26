#include "EditorUi.h"

#include <array>
#include <cstdio>
#include <filesystem>

#include <imgui.h>
#include <imgui_impl_metal.h>
#include <imgui_impl_osx.h>
#include <imgui_internal.h> // DockBuilder API, for the default panel layout

#include "EditorTheme.h"

namespace
{
constexpr const char* InspectorName = "Inspector";
constexpr const char* ViewportName = "Viewport";

constexpr float ToolbarHeight = 40.0F;
constexpr float StatusBarHeight = 28.0F;
constexpr float FontSize = 14.0F;

// macOS's system font (San Francisco). Falls back to ImGui's built-in font if missing.
constexpr const char* SystemFontPath = "/System/Library/Fonts/SFNS.ttf";

// Windows that never move, resize, dock or save state: the toolbar, the
// status bar and the invisible host window around the dockspace.
constexpr ImGuiWindowFlags FixedWindowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                              ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings |
                                              ImGuiWindowFlags_NoBringToFrontOnFocus;

// Fills the current window with a rounded island. Everything between
// BeginIsland and EndIsland is drawn inside it, with normal padding.
void BeginIsland(const char* id, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
{
  ImGui::BeginChild(id, {0.0F, 0.0F}, ImGuiChildFlags_AlwaysUseWindowPadding, flags);
}

// For fixed-height bars: content is laid out by hand, so it should never scroll
constexpr ImGuiWindowFlags BarIslandFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

void EndIsland()
{
  ImGui::EndChild();
}

// Begins a window that has no padding of its own, so its island can reach the edges
bool BeginIslandHost(const char* name, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0F, 0.0F});
  const bool isOpen = ImGui::Begin(name, nullptr, flags);
  ImGui::PopStyleVar();
  return isOpen;
}

// Vertically centers the next line of widgets in an island of the given height
void CenterRowVertically(float islandHeight)
{
  ImGui::SetCursorPosY((islandHeight - ImGui::GetFrameHeight()) * 0.5F);
}

void DrawToolbar(const ImGuiViewport& viewport)
{
  ImGui::SetNextWindowPos({viewport.WorkPos.x + theme::Gap, viewport.WorkPos.y + theme::Gap});
  ImGui::SetNextWindowSize({viewport.WorkSize.x - (2.0F * theme::Gap), ToolbarHeight});
  if (BeginIslandHost("##Toolbar", FixedWindowFlags))
  {
    BeginIsland("##ToolbarIsland", BarIslandFlags);

    // Left: which scene is open
    CenterRowVertically(ToolbarHeight);
    ImGui::AlignTextToFramePadding();
    ImGui::TextDisabled("Scene");
    ImGui::SameLine();
    ImGui::TextUnformatted("Untitled");

    // Center: play controls, disabled until there's something to play
    const float buttonWidth = ImGui::GetFrameHeight() * 2.0F;
    const float controlsWidth = (buttonWidth * 2.0F) + ImGui::GetStyle().ItemSpacing.x;
    ImGui::SameLine((ImGui::GetWindowWidth() - controlsWidth) * 0.5F);
    ImGui::BeginDisabled();
    ImGui::Button("Play", {buttonWidth, 0.0F});
    ImGui::SameLine();
    ImGui::Button("Pause", {buttonWidth, 0.0F});
    ImGui::EndDisabled();

    // Right: current mode
    constexpr const char* ModeLabel = "Edit mode";
    const float modeWidth = ImGui::CalcTextSize(ModeLabel).x;
    ImGui::SameLine(ImGui::GetWindowWidth() - modeWidth - ImGui::GetStyle().WindowPadding.x);
    ImGui::TextColored(theme::Accent, "%s", ModeLabel);

    EndIsland();
  }
  ImGui::End();
}

void DrawStatusBar(const ImGuiViewport& viewport)
{
  const float top = viewport.WorkPos.y + viewport.WorkSize.y - StatusBarHeight - theme::Gap;
  ImGui::SetNextWindowPos({viewport.WorkPos.x + theme::Gap, top});
  ImGui::SetNextWindowSize({viewport.WorkSize.x - (2.0F * theme::Gap), StatusBarHeight});

  // Tighter vertical padding than the other islands, to keep the bar slim
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {12.0F, 0.0F});
  if (BeginIslandHost("##StatusBar", FixedWindowFlags))
  {
    BeginIsland("##StatusBarIsland", BarIslandFlags);

    ImGui::SetCursorPosY((StatusBarHeight - ImGui::GetTextLineHeight()) * 0.5F);
    ImGui::TextDisabled("Ready");

    std::array<char, 32> fpsText{};
    std::snprintf(fpsText.data(), fpsText.size(), "%.0f fps", ImGui::GetIO().Framerate);
    const float fpsWidth = ImGui::CalcTextSize(fpsText.data()).x;
    ImGui::SameLine(ImGui::GetWindowWidth() - fpsWidth - ImGui::GetStyle().WindowPadding.x);
    ImGui::TextDisabled("%s", fpsText.data());

    EndIsland();
  }
  ImGui::End();
  ImGui::PopStyleVar();
}

// First-launch arrangement: inspector on the left, viewport filling the rest.
// Panels can be dragged around afterwards, like in Unity.
void BuildDefaultLayout(ImGuiID dockspaceId, ImVec2 size)
{
  ImGui::DockBuilderRemoveNode(dockspaceId);
  ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
  ImGui::DockBuilderSetNodeSize(dockspaceId, size);

  ImGuiID viewportNodeId = dockspaceId;
  const ImGuiID inspectorNodeId =
    ImGui::DockBuilderSplitNode(viewportNodeId, ImGuiDir_Left, 0.24F, nullptr, &viewportNodeId);

  // No tab bars for now: with one panel per area they add nothing, and a hidden
  // tab bar leaves a small "show tabs" triangle over the island's rounded corner.
  // Splitters still resize the panels. Revisit once areas hold several panels.
  ImGui::DockBuilderGetNode(inspectorNodeId)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
  ImGui::DockBuilderGetNode(viewportNodeId)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

  ImGui::DockBuilderDockWindow(InspectorName, inspectorNodeId);
  ImGui::DockBuilderDockWindow(ViewportName, viewportNodeId);
  ImGui::DockBuilderFinish(dockspaceId);
}

// Invisible window covering the space between toolbar and status bar,
// holding the dockspace that the inspector and viewport dock into.
void DrawDockspace(const ImGuiViewport& viewport, bool& isLayoutBuilt)
{
  const float top = viewport.WorkPos.y + theme::Gap + ToolbarHeight;
  const float height = viewport.WorkSize.y - (2.0F * theme::Gap) - ToolbarHeight - StatusBarHeight;
  ImGui::SetNextWindowPos({viewport.WorkPos.x, top});
  ImGui::SetNextWindowSize({viewport.WorkSize.x, height});

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {theme::Gap, theme::Gap});
  ImGui::Begin("##DockHost", nullptr, FixedWindowFlags | ImGuiWindowFlags_NoBackground);
  ImGui::PopStyleVar();

  const ImGuiID dockspaceId = ImGui::GetID("EditorDockspace");
  if (!isLayoutBuilt)
  {
    BuildDefaultLayout(dockspaceId, ImGui::GetContentRegionAvail());
    isLayoutBuilt = true;
  }
  ImGui::DockSpace(dockspaceId);

  ImGui::End();
}

void DrawInspector()
{
  if (BeginIslandHost(InspectorName))
  {
    BeginIsland("##InspectorIsland");
    ImGui::TextUnformatted(InspectorName);
    ImGui::Separator();
    ImGui::TextDisabled("Nothing selected");
    EndIsland();
  }
  ImGui::End();
}

void DrawViewport()
{
  if (BeginIslandHost(ViewportName))
  {
    BeginIsland("##ViewportIsland");

    // Placeholder until the scene renders into this panel
    constexpr const char* Placeholder = "Scene view";
    const ImVec2 textSize = ImGui::CalcTextSize(Placeholder);
    const ImVec2 available = ImGui::GetWindowSize();
    ImGui::SetCursorPos({(available.x - textSize.x) * 0.5F, (available.y - textSize.y) * 0.5F});
    ImGui::TextDisabled("%s", Placeholder);

    EndIsland();
  }
  ImGui::End();
}

void LoadFont()
{
  ImGuiIO& io = ImGui::GetIO();
  if (std::filesystem::exists(SystemFontPath))
  {
    io.Fonts->AddFontFromFileTTF(SystemFontPath, FontSize);
  }
  else
  {
    io.Fonts->AddFontDefault();
  }
}
} // namespace

EditorUi::EditorUi(MTL::Device* device, MTK::View* view)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  // Don't write imgui.ini (saved panel layout) into whatever directory we ran from.
  // The default layout is rebuilt every launch for now.
  io.IniFilename = nullptr;

  LoadFont();
  theme::Apply();

  ImGui_ImplMetal_Init(device);
  ImGui_ImplOSX_Init(view);
}

EditorUi::~EditorUi()
{
  ImGui_ImplMetal_Shutdown();
  ImGui_ImplOSX_Shutdown();
  ImGui::DestroyContext();
}

void EditorUi::BeginFrame(MTK::View* view, MTL::RenderPassDescriptor* passDescriptor)
{
  ImGui_ImplMetal_NewFrame(passDescriptor);
  ImGui_ImplOSX_NewFrame(view);
  ImGui::NewFrame();

  const ImGuiViewport& viewport = *ImGui::GetMainViewport();
  DrawToolbar(viewport);
  DrawDockspace(viewport, m_isLayoutBuilt);
  DrawInspector();
  DrawViewport();
  DrawStatusBar(viewport);

  ImGui::Render();
}

void EditorUi::Render(MTL::CommandBuffer* commandBuffer, MTL::RenderCommandEncoder* encoder)
{
  ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, encoder);
}

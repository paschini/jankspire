#include "EditorTheme.h"

namespace theme
{
void Apply()
{
  ImGuiStyle& style = ImGui::GetStyle();
  ImGui::StyleColorsDark(&style);

  style.WindowPadding = {12.0F, 10.0F};
  style.FramePadding = {10.0F, 5.0F};
  style.ItemSpacing = {8.0F, 6.0F};

  style.WindowRounding = IslandRounding;
  style.ChildRounding = IslandRounding;
  style.PopupRounding = 8.0F;
  style.FrameRounding = 6.0F;
  style.TabRounding = 6.0F;
  style.GrabRounding = 6.0F;
  style.ScrollbarRounding = 8.0F;

  style.WindowBorderSize = 0.0F;
  style.ChildBorderSize = 0.0F; // islands are set apart by color alone, like JetBrains'
  style.FrameBorderSize = 0.0F;

  // Docked panels are separated by a backdrop-colored splitter this wide,
  // which is what turns them into separate islands
  style.DockingSeparatorSize = Gap;

  ImVec4* colors = style.Colors;

  // Windows themselves are invisible; each panel draws its own rounded island
  // (see EditorUi.cpp), and the backdrop is the Metal view's clear color.
  colors[ImGuiCol_WindowBg] = {0.0F, 0.0F, 0.0F, 0.0F};
  colors[ImGuiCol_ChildBg] = Island;
  colors[ImGuiCol_PopupBg] = Island;
  // ImGui also paints the gaps between docked panels with the border color,
  // so it has to match the backdrop for the islands to read as separate
  colors[ImGuiCol_Border] = Backdrop;
  colors[ImGuiCol_DockingEmptyBg] = Backdrop;

  colors[ImGuiCol_Text] = Text;
  colors[ImGuiCol_TextDisabled] = TextDim;

  colors[ImGuiCol_Button] = Control;
  colors[ImGuiCol_ButtonHovered] = ControlHovered;
  colors[ImGuiCol_ButtonActive] = ControlActive;
  colors[ImGuiCol_FrameBg] = Control;
  colors[ImGuiCol_FrameBgHovered] = ControlHovered;
  colors[ImGuiCol_FrameBgActive] = ControlActive;
  colors[ImGuiCol_Header] = Control;
  colors[ImGuiCol_HeaderHovered] = ControlHovered;
  colors[ImGuiCol_HeaderActive] = ControlActive;

  colors[ImGuiCol_TitleBg] = Backdrop;
  colors[ImGuiCol_TitleBgActive] = Backdrop;
  colors[ImGuiCol_TitleBgCollapsed] = Backdrop;
  colors[ImGuiCol_Tab] = Backdrop;
  colors[ImGuiCol_TabHovered] = ControlHovered;
  colors[ImGuiCol_TabSelected] = Island;
  colors[ImGuiCol_TabSelectedOverline] = Accent;
  colors[ImGuiCol_TabDimmed] = Backdrop;
  colors[ImGuiCol_TabDimmedSelected] = Island;

  // Splitters between islands: invisible until hovered
  colors[ImGuiCol_Separator] = Backdrop;
  colors[ImGuiCol_SeparatorHovered] = Accent;
  colors[ImGuiCol_SeparatorActive] = Accent;

  colors[ImGuiCol_CheckMark] = Accent;
  colors[ImGuiCol_SliderGrab] = Accent;
  colors[ImGuiCol_SliderGrabActive] = Accent;
  colors[ImGuiCol_DockingPreview] = {Accent.x, Accent.y, Accent.z, 0.5F};
  colors[ImGuiCol_NavCursor] = Accent;
}
} // namespace theme

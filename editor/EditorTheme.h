#pragma once

#include <cstdint>

#include <imgui.h>

// Dear ImGui theme inspired by JetBrains' "Islands" UI: rounded panels
// ("islands") floating on a darker backdrop, with gaps between them.
namespace theme
{
// Builds a color from a 0xRRGGBB hex value, the way design tools show it
constexpr ImVec4 Hex(uint32_t rgb)
{
  return {
    static_cast<float>((rgb >> 16U) & 0xFFU) / 255.0F,
    static_cast<float>((rgb >> 8U) & 0xFFU) / 255.0F,
    static_cast<float>(rgb & 0xFFU) / 255.0F,
    1.0F,
  };
}

// Colors are sRGB
constexpr ImVec4 Backdrop = Hex(0x2B2D30); // behind the islands
constexpr ImVec4 Island = Hex(0x1B1C1F);   // panel surface, darker than the backdrop
constexpr ImVec4 Control = Hex(0x393B40);  // buttons and fields
constexpr ImVec4 ControlHovered = Hex(0x43454A);
constexpr ImVec4 ControlActive = Hex(0x4E5157);
constexpr ImVec4 Text = Hex(0xDFE1E5);
constexpr ImVec4 TextDim = Hex(0x868A91);
constexpr ImVec4 Accent = Hex(0x3574F0); // JetBrains blue

// Space between islands, and between islands and the window edge
constexpr float Gap = 6.0F;
constexpr float IslandRounding = 10.0F;

void Apply();
} // namespace theme

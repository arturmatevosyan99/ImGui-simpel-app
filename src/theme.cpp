#include "theme.h"

#include "imgui.h"

void ApplyShowcaseTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 10.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarRounding = 8.0f;
    style.TabRounding = 6.0f;
    style.WindowPadding = ImVec2(12.0f, 10.0f);
    style.FramePadding = ImVec2(10.0f, 6.0f);
    style.ItemSpacing = ImVec2(8.0f, 7.0f);
    style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.TabBorderSize = 0.0f;

    ImVec4* c = style.Colors;
    c[ImGuiCol_Text] = ImVec4(0.91f, 0.93f, 0.97f, 1.00f);
    c[ImGuiCol_TextDisabled] = ImVec4(0.53f, 0.56f, 0.61f, 1.00f);
    c[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.08f, 0.10f, 1.00f);
    c[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.11f, 0.14f, 1.00f);
    c[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.13f, 0.16f, 0.98f);
    c[ImGuiCol_Border] = ImVec4(0.21f, 0.24f, 0.30f, 0.18f);
    c[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.17f, 0.22f, 1.00f);
    c[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.26f, 0.34f, 1.00f);
    c[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.32f, 0.41f, 1.00f);
    c[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.11f, 0.14f, 1.00f);
    c[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.16f, 0.21f, 1.00f);
    c[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.10f, 0.13f, 1.00f);
    c[ImGuiCol_ScrollbarBg] = ImVec4(0.11f, 0.12f, 0.15f, 1.00f);
    c[ImGuiCol_ScrollbarGrab] = ImVec4(0.23f, 0.28f, 0.37f, 0.85f);
    c[ImGuiCol_CheckMark] = ImVec4(0.44f, 0.77f, 0.99f, 1.00f);
    c[ImGuiCol_SliderGrab] = ImVec4(0.44f, 0.77f, 0.99f, 0.86f);
    c[ImGuiCol_SliderGrabActive] = ImVec4(0.63f, 0.84f, 1.00f, 1.00f);
    c[ImGuiCol_Button] = ImVec4(0.18f, 0.24f, 0.33f, 1.00f);
    c[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.35f, 0.49f, 1.00f);
    c[ImGuiCol_ButtonActive] = ImVec4(0.32f, 0.44f, 0.60f, 1.00f);
    c[ImGuiCol_Header] = ImVec4(0.19f, 0.25f, 0.35f, 1.00f);
    c[ImGuiCol_HeaderHovered] = ImVec4(0.27f, 0.38f, 0.52f, 1.00f);
    c[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.49f, 0.65f, 1.00f);
    c[ImGuiCol_Separator] = ImVec4(0.25f, 0.29f, 0.37f, 0.40f);
    c[ImGuiCol_ResizeGrip] = ImVec4(0.44f, 0.77f, 0.99f, 0.35f);
    c[ImGuiCol_Tab] = ImVec4(0.16f, 0.20f, 0.29f, 1.00f);
    c[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.47f, 0.64f, 1.00f);
    c[ImGuiCol_TabActive] = ImVec4(0.24f, 0.34f, 0.47f, 1.00f);
    c[ImGuiCol_PlotLines] = ImVec4(0.46f, 0.77f, 0.98f, 1.00f);
    c[ImGuiCol_PlotHistogram] = ImVec4(0.97f, 0.71f, 0.29f, 1.00f);
}

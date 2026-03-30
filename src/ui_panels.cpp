#include "ui_panels.h"

#include "imgui.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace {
float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

void RenderToolbar(AppState& state) {
    ImGui::TextUnformatted("Creative Tool UI - Showcase");
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 240.0f);

    const float pulse = 0.78f + 0.22f * std::sin(state.playback_time * 4.0f);
    if (state.is_playing) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.57f, 0.36f, pulse));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.32f, 0.66f, 0.43f, 1.0f));
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.50f, 0.25f, 0.20f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.60f, 0.31f, 0.24f, 1.0f));
    }

    if (ImGui::Button(state.is_playing ? "Pause" : "Play")) {
        TogglePlayback(state);
    }
    ImGui::PopStyleColor(2);

    ImGui::SameLine();
    if (ImGui::Button("Add Marker")) {
        PushEvent(state, "Marker added at " + std::to_string(state.playback_time).substr(0, 4) + "s");
        PushNotification(state, "Marker added");
    }
    ImGui::SameLine();
    if (ImGui::Button("Export")) {
        PushEvent(state, "Export queued (preview)");
        PushNotification(state, "Export started");
    }
}

void RenderAssetBrowser(AppState& state) {
    ImGui::TextUnformatted("Asset Browser");
    ImGui::Separator();

    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputTextWithHint("##search", "Search assets...", state.search_buffer, sizeof(state.search_buffer));
    ImGui::Spacing();

    for (int i = 0; i < static_cast<int>(state.assets.size()); ++i) {
        AssetItem& item = state.assets[i];
        const bool selected = (state.selected_asset == i);

        ImVec4 bg = ImVec4(
            Lerp(0.13f, 0.30f, item.select_fade),
            Lerp(0.14f, 0.42f, item.select_fade),
            Lerp(0.18f, 0.58f, item.select_fade),
            Lerp(0.35f, 0.75f, item.select_fade));
        ImGui::PushStyleColor(ImGuiCol_Header, bg);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(bg.x + 0.08f, bg.y + 0.08f, bg.z + 0.08f, 0.95f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(bg.x + 0.12f, bg.y + 0.12f, bg.z + 0.12f, 1.0f));

        std::string label = item.name + "##asset_" + std::to_string(i);
        if (ImGui::Selectable(label.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns)) {
            SelectAsset(state, i);
        }

        ImGui::SameLine(ImGui::GetWindowWidth() - 90.0f);
        ImGui::TextDisabled("%s", item.kind.c_str());

        ImGui::PopStyleColor(3);
    }
}

void RenderTimeline(AppState& state) {
    ImGui::TextUnformatted("Timeline");
    ImGui::Separator();

    const float content_w = ImGui::GetContentRegionAvail().x;
    const float track_h = 180.0f;
    const ImVec2 origin = ImGui::GetCursorScreenPos();
    ImDrawList* draw = ImGui::GetWindowDrawList();

    draw->AddRectFilled(origin, ImVec2(origin.x + content_w, origin.y + track_h), IM_COL32(24, 29, 38, 255), 8.0f);
    draw->AddRect(origin, ImVec2(origin.x + content_w, origin.y + track_h), IM_COL32(95, 109, 130, 70), 8.0f);

    for (size_t i = 0; i < state.clips.size(); ++i) {
        TimelineClip& clip = state.clips[i];
        const float x0 = origin.x + (clip.start_time / state.timeline_length) * content_w;
        const float x1 = origin.x + ((clip.start_time + clip.duration) / state.timeline_length) * content_w;
        const float y0 = origin.y + 18.0f + (i % 4) * 38.0f;
        const float y1 = y0 + 28.0f;

        ImVec4 hsv = ImColor::HSV(clip.color_hue + state.accent_shift, 0.55f, 0.78f);
        ImU32 base = ImGui::ColorConvertFloat4ToU32(ImVec4(hsv.x, hsv.y, hsv.z, 0.82f));
        ImU32 hover = ImGui::ColorConvertFloat4ToU32(ImVec4(
            std::min(hsv.x + 0.20f * clip.hover_fade, 1.0f),
            std::min(hsv.y + 0.18f * clip.hover_fade, 1.0f),
            std::min(hsv.z + 0.22f * clip.hover_fade, 1.0f),
            0.95f));
        draw->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), hover ? hover : base, 6.0f);

        const bool inside = ImGui::IsMouseHoveringRect(ImVec2(x0, y0), ImVec2(x1, y1));
        clip.hover_fade = Lerp(clip.hover_fade, inside ? 1.0f : 0.0f, 0.14f);
        if (inside && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            PushEvent(state, "Clip focused: " + clip.name);
            PushNotification(state, "Timeline clip focused");
        }

        draw->AddText(ImVec2(x0 + 6.0f, y0 + 6.0f), IM_COL32(230, 236, 246, 255), clip.name.c_str());
    }

    const float playhead_x = origin.x + (state.playback_time / state.timeline_length) * content_w;
    draw->AddLine(ImVec2(playhead_x, origin.y), ImVec2(playhead_x, origin.y + track_h), IM_COL32(255, 194, 92, 240), 2.0f);

    ImGui::InvisibleButton("timeline_surface", ImVec2(content_w, track_h + 8.0f));
    ImGui::Spacing();

    float normalized = state.playback_time / state.timeline_length;
    ImGui::ProgressBar(normalized, ImVec2(-1.0f, 10.0f), "");
    ImGui::Text("Playhead: %.2fs / %.2fs", state.playback_time, state.timeline_length);
}

void RenderProperties(AppState& state) {
    const AssetItem& item = state.assets[state.selected_asset];
    ImGui::TextUnformatted("Properties");
    ImGui::Separator();
    ImGui::Text("Name: %s", item.name.c_str());
    ImGui::Text("Type: %s", item.kind.c_str());
    ImGui::Text("Duration: %.2fs", item.duration_seconds);
    ImGui::Spacing();

    static float intensity = 0.65f;
    static float exposure = 0.20f;
    static bool motion_blur = true;
    static bool bloom = true;

    ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f);
    ImGui::SliderFloat("Exposure", &exposure, -1.0f, 1.0f);
    ImGui::Checkbox("Motion Blur", &motion_blur);
    ImGui::Checkbox("Bloom", &bloom);

    if (ImGui::Button("Apply Preset")) {
        PushEvent(state, "Preset applied to: " + item.name);
        PushNotification(state, "Preset applied");
    }
    ImGui::SameLine();
    if (ImGui::Button("Queue Render")) {
        PushEvent(state, "Render queued: " + item.name);
        PushNotification(state, "Render queued");
    }
}

void RenderEventLog(AppState& state) {
    ImGui::TextUnformatted("Event Log");
    ImGui::Separator();
    for (const std::string& event : state.event_log) {
        ImGui::BulletText("%s", event.c_str());
    }
}

void RenderFloatingOverlays(const AppState& state) {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 base = ImVec2(viewport->Pos.x + viewport->Size.x - 310.0f, viewport->Pos.y + 70.0f);

    ImGui::SetNextWindowPos(base, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.55f);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    if (ImGui::Begin("PerfOverlay", nullptr, flags)) {
        ImGui::Text("FPS %.1f", state.fps_smoothed);
        ImGui::Text("Assets %d", static_cast<int>(state.assets.size()));
        ImGui::Text("Clips %d", static_cast<int>(state.clips.size()));
    }
    ImGui::End();

    float y_offset = 110.0f;
    for (const Notification& note : state.notifications) {
        const float t = (note.max_ttl > 0.0f) ? (note.ttl / note.max_ttl) : 0.0f;
        const float alpha = std::clamp(t, 0.0f, 1.0f);
        ImGui::SetNextWindowPos(ImVec2(base.x, viewport->Pos.y + y_offset), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.25f + 0.50f * alpha);
        if (ImGui::Begin(("Toast##" + note.text).c_str(), nullptr, flags)) {
            ImGui::TextUnformatted(note.text.c_str());
        }
        ImGui::End();
        y_offset += 38.0f;
    }
}
}  // namespace

void RenderCreativeShowcase(AppState& state, float dt) {
    (void)dt;
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    const ImVec2 p = vp->WorkPos;
    const ImVec2 s = vp->WorkSize;

    const float toolbar_h = 56.0f;
    const float bottom_h = 160.0f;
    const float left_w = std::max(240.0f, s.x * 0.22f);
    const float right_w = std::max(260.0f, s.x * 0.26f);
    const float center_h = s.y - toolbar_h - bottom_h;
    const ImGuiWindowFlags panel_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

    ImGui::SetNextWindowPos(ImVec2(p.x, p.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(s.x, toolbar_h), ImGuiCond_Always);
    if (ImGui::Begin("Toolbar", nullptr, panel_flags | ImGuiWindowFlags_NoTitleBar)) {
        RenderToolbar(state);
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(p.x, p.y + toolbar_h), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(left_w, center_h), ImGuiCond_Always);
    if (ImGui::Begin("Assets", nullptr, panel_flags)) {
        RenderAssetBrowser(state);
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(p.x + left_w, p.y + toolbar_h), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(s.x - left_w - right_w, center_h), ImGuiCond_Always);
    if (ImGui::Begin("TimelinePanel", nullptr, panel_flags)) {
        RenderTimeline(state);
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(p.x + s.x - right_w, p.y + toolbar_h), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(right_w, center_h), ImGuiCond_Always);
    if (ImGui::Begin("PropertiesPanel", nullptr, panel_flags)) {
        RenderProperties(state);
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(p.x, p.y + s.y - bottom_h), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(s.x, bottom_h), ImGuiCond_Always);
    if (ImGui::Begin("EventLogPanel", nullptr, panel_flags)) {
        RenderEventLog(state);
    }
    ImGui::End();

    RenderFloatingOverlays(state);
}

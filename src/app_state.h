#pragma once

#include <deque>
#include <string>
#include <vector>

struct AssetItem {
    std::string name;
    std::string kind;
    float duration_seconds = 0.0f;
    float select_fade = 0.0f;
};

struct TimelineClip {
    std::string name;
    float start_time = 0.0f;
    float duration = 0.0f;
    float color_hue = 0.0f;
    float hover_fade = 0.0f;
};

struct Notification {
    std::string text;
    float ttl = 0.0f;
    float max_ttl = 0.0f;
};

struct AppState {
    std::vector<AssetItem> assets;
    std::vector<TimelineClip> clips;
    std::deque<std::string> event_log;
    std::vector<Notification> notifications;

    int selected_asset = 0;
    bool is_playing = true;
    float playback_time = 0.0f;
    float timeline_length = 20.0f;
    float accent_shift = 0.12f;
    float fps_smoothed = 60.0f;
    char search_buffer[64] = {};
};

void InitializeAppState(AppState& state);
void UpdateAppState(AppState& state, float dt);
void TogglePlayback(AppState& state);
void SelectAsset(AppState& state, int index);
void PushEvent(AppState& state, const std::string& text);
void PushNotification(AppState& state, const std::string& text);

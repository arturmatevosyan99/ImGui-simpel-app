#include "app_state.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace {
float Damp(float current, float target, float dt, float speed) {
    const float k = 1.0f - std::exp(-speed * dt);
    return current + (target - current) * k;
}
}  // namespace

void InitializeAppState(AppState& state) {
    state.assets = {
        {"HeroCharacter", "Model", 8.5f},
        {"CityAmbience", "Audio", 12.0f},
        {"ImpactGlow", "Effect", 3.4f},
        {"TitleCard", "Texture", 2.6f},
        {"CameraPath_A", "Animation", 9.2f},
        {"Dialogue_EN", "Voice", 11.3f},
    };

    state.clips = {
        {"Shot_Intro", 0.0f, 4.2f, 0.58f},
        {"LowerThird", 1.5f, 2.1f, 0.06f},
        {"HeroReveal", 4.1f, 5.0f, 0.79f},
        {"ColorGrade", 5.0f, 3.5f, 0.35f},
        {"OutroLogo", 10.2f, 3.0f, 0.92f},
    };

    state.selected_asset = 0;
    state.is_playing = true;
    state.playback_time = 0.0f;
    state.timeline_length = 18.0f;
    state.accent_shift = 0.12f;
    state.fps_smoothed = 60.0f;
    PushEvent(state, "Project loaded: CreativeTool_Showcase");
    PushEvent(state, "Auto-play enabled");
    PushNotification(state, "Session initialized");
}

void UpdateAppState(AppState& state, float dt) {
    dt = std::max(0.0f, std::min(dt, 0.05f));
    if (state.is_playing) {
        state.playback_time += dt * 1.0f;
        while (state.playback_time > state.timeline_length) {
            state.playback_time -= state.timeline_length;
            PushEvent(state, "Playback looped");
        }
    }

    state.fps_smoothed = Damp(state.fps_smoothed, (dt > 0.0f ? 1.0f / dt : 60.0f), dt, 3.0f);
    state.accent_shift = Damp(state.accent_shift, 0.10f + 0.08f * std::sin(state.playback_time * 0.6f), dt, 4.0f);

    for (size_t i = 0; i < state.assets.size(); ++i) {
        const float target = (static_cast<int>(i) == state.selected_asset) ? 1.0f : 0.0f;
        state.assets[i].select_fade = Damp(state.assets[i].select_fade, target, dt, 10.0f);
    }

    for (auto it = state.notifications.begin(); it != state.notifications.end();) {
        it->ttl -= dt;
        if (it->ttl <= 0.0f) {
            it = state.notifications.erase(it);
        } else {
            ++it;
        }
    }
}

void TogglePlayback(AppState& state) {
    state.is_playing = !state.is_playing;
    PushEvent(state, state.is_playing ? "Playback resumed" : "Playback paused");
    PushNotification(state, state.is_playing ? "Play" : "Pause");
}

void SelectAsset(AppState& state, int index) {
    if (index < 0 || index >= static_cast<int>(state.assets.size()) || index == state.selected_asset) {
        return;
    }
    state.selected_asset = index;
    PushEvent(state, "Selected asset: " + state.assets[index].name);
    PushNotification(state, "Active asset changed");
}

void PushEvent(AppState& state, const std::string& text) {
    state.event_log.push_front(text);
    while (state.event_log.size() > 14) {
        state.event_log.pop_back();
    }
}

void PushNotification(AppState& state, const std::string& text) {
    state.notifications.push_back({text, 2.6f, 2.6f});
}

# ImGui Creative Showcase

Simple but professional demo project built with Dear ImGui + GLFW + OpenGL + CMake.

This app is designed as a portfolio-ready "Creative Tool UI" that demonstrates:
- multi-panel layout (toolbar, asset browser, timeline, properties, event log),
- smooth UI animations (selection fades, timeline playhead, notification toasts),
- clean C++ structure suitable for client handoff.

## Tech Stack
- Dear ImGui
- GLFW
- OpenGL2 backend for ImGui rendering
- CMake

## Build (Windows, Visual Studio)
```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

Run:
```powershell
.\build\Release\imgui_creative_showcase.exe
```

## Build (Generic CMake flow)
```bash
cmake -S . -B build
cmake --build build -j
./build/imgui_creative_showcase
```

## Code Handoff Notes
- Entry point: `src/main.cpp`
- State and animation data: `src/app_state.h` and `src/app_state.cpp`
- UI layout/panels: `src/ui_panels.cpp`
- Visual style: `src/theme.cpp`

Project dependencies are fetched automatically by CMake via `FetchContent`.
cmake_minimum_required(VERSION 3.20)
project(imgui_creative_showcase LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# GLFW for window/input and Dear ImGui for immediate-mode UI.
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

set(IMGUI_LOCAL_DIR "${CMAKE_SOURCE_DIR}/third_party/imgui-1.92.6")
if(EXISTS "${IMGUI_LOCAL_DIR}/imgui.h")
    set(imgui_SOURCE_DIR "${IMGUI_LOCAL_DIR}")
    message(STATUS "Using local Dear ImGui from: ${imgui_SOURCE_DIR}")
else()
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.6
    )
    FetchContent_MakeAvailable(imgui)
endif()

FetchContent_MakeAvailable(glfw)

add_library(imgui_framework STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp
)

target_include_directories(imgui_framework PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_framework PUBLIC glfw)

add_executable(imgui_creative_showcase
    src/main.cpp
    src/app_state.cpp
    src/ui_panels.cpp
    src/theme.cpp
)

target_include_directories(imgui_creative_showcase PRIVATE src)
target_link_libraries(imgui_creative_showcase PRIVATE imgui_framework)

if(WIN32)
    target_link_libraries(imgui_creative_showcase PRIVATE opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(imgui_creative_showcase PRIVATE OpenGL::GL)
endif()

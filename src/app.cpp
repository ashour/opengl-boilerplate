#include "app.h"
#include "config.h"
#include "labs/blending_lab.h"
#include "system/input.h"
#include "system/time.h"
#include "system/window.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <memory>

namespace eo
{
void App::run()
{
    EO_LOG_INFO("Initializing OpenGL");
    _window = std::make_unique<Window>(
        WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
    _lab = std::make_unique<BlendingLab>(*_window);
    EO_LOG_INFO("OpenGL version {}", (const char*)_window->opengl_version());

    EO_LOG_INFO("Initializing ImGui");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window->native_window(), true);
    ImGui_ImplOpenGL3_Init();

    EO_LOG_HEADING("Initialization Complete");

    loop();
}

void App::loop()
{
    bool is_running = true;
    while (is_running)
    {
        Time::update();
        _window->poll_events();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        _window->clear();

        _lab->on_update();
        _lab->on_render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        _window->swap_buffers();
        is_running = !(_window->should_close() || Input::action_pressed(Action::quit_app));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
} // namespace eo

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
    _ui = std::make_unique<UI>(_window->native_window());

    EO_LOG_HEADING("Initialization Complete");

    loop();
}

void App::loop()
{
    glm::vec4 clear_color{0.5f, 0.5f, 0.5f, 1.0f};

    bool is_running = true;
    while (is_running)
    {
        Time::update();
        _window->poll_events();

        _ui->new_frame();

        {
            ImGui::Begin("Clear colour");
            ImGui::ColorEdit4("Color", &clear_color.x);
            ImGui::End();
        }

        _window->set_clear_color(clear_color);
        _window->clear();

        _lab->on_update();
        _lab->on_render();
        // _lab->on_imgui_render();

        _ui->render();
        _window->swap_buffers();
        is_running = !(_window->should_close() || Input::action_pressed(Action::quit_app));
    }
}
} // namespace eo

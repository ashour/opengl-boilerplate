#include "app.h"
#include "config.h"
#include "labs/blending_lab.h"
#include "labs/registry/lab_registry.h"
#include "system/input.h"
#include "system/time.h"
#include "system/window.h"
#include <format>
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
    _lab = new BlendingLab(*_window);
    EO_LOG_INFO("OpenGL version {}", (const char*)_window->opengl_version());

    EO_LOG_INFO("Initializing ImGui");
    _ui = std::make_unique<UI>(_window->native_window());

    EO_LOG_HEADING("Initialization Complete");

    loop();
}

void App::loop()
{
    bool is_running = true;
    static std::string current_lab = "Blending";

    while (is_running)
    {
        Time::update();
        _window->poll_events();
        _lab->on_update();

        _ui->new_frame();

        {
            _ui->begin_window(std::format("Labs [{}]", current_lab));
            for (const auto& pair : LabRegistry::labs())
            {
                auto name = pair.first;
                bool is_selected = (current_lab == name);
                if (_ui->radio_button(name, is_selected))
                {
                    if (current_lab != name)
                    {
                        current_lab = name;
                        delete _lab;
                        _lab = LabRegistry::create(name, *_window);
                    }
                }
            }
            _ui->end_window();
        }

        _window->clear();

        _lab->on_render();

        // _lab->on_gui_render(_ui);
        _ui->render();

        _window->swap_buffers();
        is_running = !(_window->should_close() || Input::key_pressed(Key::escape));
    }
}
} // namespace eo

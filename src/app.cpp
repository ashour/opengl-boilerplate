#include "app.h"
#include "config.h"
#include "labs/framebuffer_lab.h"
#include "labs/registry/lab_registry.h"
#include "system/input.h"
#include "system/time.h"
#include "system/window.h"
#include <format>
#include <memory>

namespace eo
{
void App::run()
{
    EO_LOG_INFO("Initializing OpenGL");
    _window = std::make_unique<Window>(
        WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
    _current_lab_name = "Framebuffers";
    _lab = new FramebufferLab(*_window);
    EO_LOG_INFO("OpenGL version {}", (const char*)_window->opengl_version());

    EO_LOG_INFO("Initializing ImGui");
    _ui = std::make_unique<UI>(_window->native_window());

    _metrics = std::make_unique<Metrics>();

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
        _lab->on_update();
        _ui->new_frame();
        _window->clear();

        if (SHOW_UI_METRICS)
        {
            Metrics::clear_drawn_vertex_count();
            _metrics->begin_fragment_count_query();
        }

        _lab->on_render();

        if (SHOW_UI_METRICS)
        {
            _fragment_count = _metrics->end_fragment_count_query();
        }

        lab_selector();
        if (SHOW_UI_DEMO)
        {
            _ui->show_demo();
        }
        if (SHOW_UI_METRICS)
        {
            _ui->show_metrics(_fragment_count, Metrics::drawn_vertex_count());
        }
        _lab->on_ui_render(*_ui);
        _ui->render();
        _window->swap_buffers();
        is_running = !(_window->should_close() || Input::key_pressed(Key::escape));
    }
}

void App::lab_selector()
{
    _ui->begin_window(std::format("Labs [{}]", _current_lab_name));
    for (const auto& pair : LabRegistry::labs())
    {
        auto name = pair.first;
        bool is_selected = (_current_lab_name == name);
        if (_ui->radio_button(name, is_selected))
        {
            if (_current_lab_name != name)
            {
                _current_lab_name = name;
                delete _lab;
                _lab = LabRegistry::create(name, *_window);
            }
        }
    }
    _ui->end_window();
}
} // namespace eo

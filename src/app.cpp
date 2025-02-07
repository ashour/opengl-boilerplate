#include "app.h"
#include "config.h"
#include "labs/blending_lab.h"
#include "system/input.h"
#include "system/time.h"
#include "system/window.h"
#include <memory>

namespace eo
{
void App::run()
{
    _window = std::make_unique<Window>(
        WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
    _lab = std::make_unique<BlendingLab>(*_window);

    EO_LOG_HEADING("Initialization Complete");
    EO_LOG_INFO("OpenGL version {}", (const char*)_window->opengl_version());

    loop();
}

void App::loop()
{
    bool is_running = true;
    while (is_running)
    {
        Time::update();

        _window->poll_events();
        _window->clear();

        _lab->on_update();
        _lab->on_render();

        _window->swap_buffers();
        is_running = !(_window->should_close() || Input::action_pressed(Action::quit_app));
    }
}
} // namespace eo

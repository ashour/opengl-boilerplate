#include "app.h"
#include "lib/color.h"
#include "lib/log.h"
#include "objects/cube.h"
#include "objects/plane.h"
#include "rendering/camera.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "rendering/transform.h"
#include "system/input.h"
#include "system/time.h"
#include "system/window.h"
#include <glm/glm.hpp>
#include <memory>

namespace eo
{
int App::run()
{
    if (!init_window())
    {
        return -1;
    }
    init_rendering();
    init_input();

    LOG_H("Initialization Complete");
    LOG("OpenGL version " << _window->opengl_version());

    loop();
    return 0;
}

bool App::init_window()
{
    try
    {
        _window = std::make_unique<Window>(
            WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
    }
    catch (const WindowException& e)
    {
        LOG_ERR(e.what());
        return false;
    }

    return true;
}

void App::init_rendering()
{
    _window->set_clear_color(glm::vec4{NCOLV(36.0), NCOLV(22.0), NCOLV(35), 1.0});

    _shader = std::make_unique<Shader>(VERTEX_SHADER_FILEPATH, FRAGMENT_SHADER_FILEPATH);
    _shader->build();

    _shader->use();
    _u_view = _shader->uniform_location_for("u_view");
    unsigned int u_projection = _shader->uniform_location_for("u_projection");

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window->buffer_width() / static_cast<float>(_window->buffer_height())));

    _shader->set_uniform_mat4(u_projection, _camera->projection());
    Shader::unuse_all();
}

void App::init_input()
{
    Input::register_mouse_move_handler(
        [this](auto current_mouse_position, auto last_mouse_position)
        { _camera->look(current_mouse_position, last_mouse_position); });
}

void App::loop()
{
    bool is_running = true;
    while (is_running)
    {
        Time::update();

        _window->poll_events();
        _window->clear();

        handle_input();

        render_scene();
        _window->swap_buffers();

        is_running = !(_window->should_close() || Input::action_pressed(Action::quit_app));
    }
}

void App::handle_input()
{
    if (Input::action_pressed(Action::move_forward))
    {
        _camera->strafe(Strafe::forward, Time::delta_time());
    }
    if (Input::action_pressed(Action::move_back))
    {
        _camera->strafe(Strafe::back, Time::delta_time());
    }
    if (Input::action_pressed(Action::move_left))
    {
        _camera->strafe(Strafe::left, Time::delta_time());
    }
    if (Input::action_pressed(Action::move_right))
    {
        _camera->strafe(Strafe::right, Time::delta_time());
    }
}

void App::render_scene()
{
    _shader->use();
    _shader->set_uniform_mat4(_u_view, _camera->view());

    Mesh cube{eo::cube};
    Transform cube_transform{};
    cube_transform.position({0.0f, 7.5f, -5.0f});
    cube_transform.rotation(static_cast<float>(glfwGetTime()) * glm::radians(50.0f),
                            {0.5f, 1.0f, 0.0f});
    unsigned int u_model = _shader->uniform_location_for("u_model");
    _shader->set_uniform_mat4(u_model, cube_transform.matrix());
    cube.draw();

    cube_transform.position({2.0f, 7.5f, -5.0f});
    _shader->set_uniform_mat4(u_model, cube_transform.matrix());
    cube.draw();

    Mesh plane{eo::plane};
    _shader->set_uniform_mat4(u_model, glm::mat4{1.0f});

    plane.draw();
}
} // namespace eo

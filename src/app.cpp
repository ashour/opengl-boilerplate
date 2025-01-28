#include "app.h"
#include "config.h"
#include "lib/color.h"
#include "lib/log.h"
#include "lib/random.h"
#include "objects/primitive.h"
#include "rendering/camera.h"
#include "rendering/lights/point_light.h"
#include "rendering/lights/spot_light.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "rendering/transform.h"
#include "system/input.h"
#include "system/time.h"
#include "system/window.h"
#include <glm/ext.hpp>
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
    init_cube_positions();

    EO_LOG_HEADING("Initialization Complete");
    EO_LOG_INFO("OpenGL version " << _window->opengl_version());

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
        EO_LOG_ERROR(e.what());
        return false;
    }

    return true;
}

void App::init_rendering()
{
    _window->set_clear_color(SCENE_CLEAR_COLOR);

    _shader =
        std::make_shared<Shader>(DEFAULT_VERTEX_SHADER_FILEPATH, DEFAULT_FRAGMENT_SHADER_FILEPATH);
    _shader->build();

    _shader->use();
    _u_view_matrix = _shader->uniform_location_for("u_view");
    _u_view_position = _shader->uniform_location_for("u_view_position");

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window->buffer_width() / static_cast<float>(_window->buffer_height())));

    _shader->set_uniform("u_projection", _camera->projection());

    _directional_light = std::make_unique<DirectionalLight>(*_shader,
                                                            "u_directional_light",
                                                            glm::vec3(-5.0f, -1.0f, -10.0f),
                                                            EO_COLOR3_HEX(0x011C33),
                                                            EO_COLOR3_HEX(0x97E8EC),
                                                            EO_COLOR3_HEX(0xE5FAFF));

    PointLight{*_shader,
               "u_point_light",
               EO_COLOR3_HEX(0x8ECAE6),
               EO_COLOR3_HEX(0x8ECAE6),
               EO_COLOR3_HEX(0xE5FAFF),
               glm::vec3(0.0f, 4.0f, 0.0f),
               1.0f,
               0.045f,
               0.0075f};

    _flash_light = std::make_unique<SpotLight>(*_shader,
                                               "u_spot_light",
                                               glm::vec3(0.0f),
                                               EO_COLOR3_HEX(0xFFB703),
                                               EO_COLOR3_HEX(0xFEEEC2),
                                               _camera->position(),
                                               1.0f,
                                               0.007f,
                                               0.0002f,
                                               _camera->front(),
                                               glm::cos(glm::radians(12.5f)),
                                               glm::cos(glm::radians(17.5f)));

    _mat_dirt = std::make_unique<Material>(*_shader, TEXTURE_DIR + "dirt.png", Format::RGBA, 25.0f);

    _mat_box = std::make_unique<Material>(*_shader,
                                          TEXTURE_DIR + "container2_diffuse.png",
                                          Format::RGBA,
                                          400.0f,
                                          TEXTURE_DIR + "container2_specular.png",
                                          Format::RGBA);
    Shader::unuse_all();

    _plane = std::make_unique<Mesh>(Primitive::plane());
    _cube = std::make_unique<Mesh>(Primitive::cube());
}

void App::init_input()
{
    Input::register_mouse_move_handler(
        [this](auto current_mouse_position, auto last_mouse_position)
        { _camera->look(current_mouse_position, last_mouse_position); });
}

void App::init_cube_positions()
{
    for (size_t i = 0; i < _cube_positions.size(); i += 1)
    {
        _cube_positions[i] = {
            random_float(-90.0f, 90.0f),
            random_float(2.0f, 20.0f),
            random_float(-90.0f, 90.0f),
        };
    }
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
    _shader->set_uniform(_u_view_matrix, _camera->view());
    _shader->set_uniform(_u_view_position, _camera->position());

    constexpr float light_orbit_radius = 50.0f;
    float light_y_tilt = glm::tan(glm::radians(-30.0f));
    float light_angle = std::fmod(glm::radians(Time::current_time() * 20.0f), glm::two_pi<float>());
    _directional_light->set_direction(glm::vec3(light_orbit_radius * glm::cos(light_angle),
                                                light_orbit_radius * light_y_tilt,
                                                light_orbit_radius * glm::sin(light_angle)));

    _flash_light->set_position(_camera->position());
    _flash_light->set_direction(_camera->front());

    Transform plane_transform{};
    plane_transform.scale(glm::vec3(200.0f, 1.0f, 200.0f));
    _shader->set_uniform("u_model", plane_transform.matrix());
    _shader->set_uniform("u_texture_scale", 0.02f);
    _mat_dirt->use();
    _plane->draw();

    Transform cube_transform{};
    cube_transform.rotation(Time::current_time() * glm::radians(50.0f), {0.5f, 1.0f, 0.0f});
    _shader->set_uniform("u_texture_scale", 1.0f);
    _mat_box->use();

    for (glm::vec3 position : _cube_positions)
    {
        cube_transform.position(position);
        _shader->set_uniform("u_model", cube_transform.matrix());
        _cube->draw();
    }
}
} // namespace eo

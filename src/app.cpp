#include "app.h"
#include "config.h"
#include "lib/color.h"
#include "lib/log.h"
#include "lib/random.h"
#include "objects/primitive.h"
#include "rendering/camera.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "rendering/transform.h"
#include "system/input.h"
#include "system/time.h"
#include "system/window.h"
#include <cmath>
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

    auto u_projection = _shader->uniform_location_for("u_projection");
    _shader->set_uniform_mat4(u_projection, _camera->projection());

    _light = std::make_unique<Light>(*_shader,
                                     glm::vec3(10.0f, 50.0f, -10.0f),
                                     glm::vec3(0.4f),
                                     glm::vec3(0.8f),
                                     glm::vec3(EO_NCOLV(255), EO_NCOLV(204), EO_NCOLV(107)));

    _mat_green_clay = std::make_unique<Material>(
        *_shader, glm::vec3(0.1f, 0.35f, 0.1f), glm::vec3(0.45f, 0.55f, 0.45f), 25.0f);

    _mat_gold = std::make_unique<Material>(*_shader,
                                           glm::vec3(0.75164f, 0.60648f, 0.22648f),
                                           glm::vec3(0.628281f, 0.555802f, 0.366065f),
                                           400.0f);

    auto u_material_diffuse = _shader->uniform_location_for("u_material.diffuse");
    _shader->set_uniform_1i(u_material_diffuse, 0);
    auto u_material_specular = _shader->uniform_location_for("u_material.specular");
    _shader->set_uniform_1i(u_material_specular, 1);

    Shader::unuse_all();

    _tex_box_diffuse =
        std::make_unique<Texture>(TEXTURE_DIR + "container2_diffuse.png", Format::RGBA);
    _tex_box_specular =
        std::make_unique<Texture>(TEXTURE_DIR + "container2_specular.png", Format::RGBA);
    _tex_dirt_diffuse = std::make_unique<Texture>(TEXTURE_DIR + "dirt.png", Format::RGBA);

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
    _shader->set_uniform_mat4(_u_view_matrix, _camera->view());
    _shader->set_uniform_vec3(_u_view_position, _camera->position());

    constexpr float light_orbit_radius = 50.0f;
    float light_angle = std::fmod(glm::radians(Time::current_time() * 20.0f), glm::two_pi<float>());
    _light->set_position(glm::vec3(light_orbit_radius * glm::cos(light_angle),
                                   light_orbit_radius * glm::sin(light_angle),
                                   0.0f));

    unsigned int u_model = _shader->uniform_location_for("u_model");
    unsigned int u_texture_scale = _shader->uniform_location_for("u_texture_scale");

    Transform plane_transform{};
    plane_transform.scale(glm::vec3(200.0f, 1.0f, 200.0f));
    _shader->set_uniform_mat4(u_model, plane_transform.matrix());
    _shader->set_uniform_1f(u_texture_scale, 0.02f);
    _mat_green_clay->use();
    _tex_dirt_diffuse->bind(TextureUnit::TEXUNIT0);
    _tex_dirt_diffuse->bind(TextureUnit::TEXUNIT1);
    _plane->draw();

    Transform cube_transform{};
    cube_transform.rotation(Time::current_time() * glm::radians(50.0f), {0.5f, 1.0f, 0.0f});
    _shader->set_uniform_1f(u_texture_scale, 1.0f);
    _mat_gold->use();
    _tex_box_diffuse->bind(TextureUnit::TEXUNIT0);
    _tex_box_specular->bind(TextureUnit::TEXUNIT1);

    for (glm::vec3 position : _cube_positions)
    {
        cube_transform.position(position);
        _shader->set_uniform_mat4(u_model, cube_transform.matrix());
        _cube->draw();
    }
}
} // namespace eo

#include "config.h"
#include "lib/random.h"
#include "objects/primitive.h"
#include "phong_lab.h"
#include "registry/lab_registry.h"
#include "rendering/lights/point_light.h"
#include "rendering/transform.h"
#include "system/time.h"
#include <format>

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab(
        "Phong shading", [](eo::Window& window) -> eo::Lab* { return new eo::PhongLab(window); });
    return true;
}();
} // namespace

namespace eo
{

PhongLab::PhongLab(const Window& window) : Lab(window)
{
    _window.set_clear_color(SCENE_CLEAR_COLOR);

    _shader =
        std::make_shared<Shader>(DEFAULT_VERTEX_SHADER_FILEPATH, DEFAULT_FRAGMENT_SHADER_FILEPATH);
    _shader->build();

    _shader->use();
    _u_view_matrix = _shader->uniform_location_for("u_view");
    _u_view_position = _shader->uniform_location_for("u_view_position");

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _shader->set_uniform("u_projection", _camera->projection());

    _directional_light = std::make_unique<DirectionalLight>(*_shader,
                                                            "u_directional_light",
                                                            glm::vec3(-5.0f, -1.0f, -10.0f),
                                                            EO_COLOR3_HEX(0x011C33),
                                                            EO_COLOR3_HEX(0x97E8EC),
                                                            EO_COLOR3_HEX(0xE5FAFF));

    std::vector<glm::vec3> point_light_data = {
        glm::vec3(12.0f, 0.2f, 4.0f),
        EO_COLOR3_HEX(0x8ECAE6),
        EO_COLOR3_HEX(0x8ECAE6),
        EO_COLOR3_HEX(0xE5FAFF),
        glm::vec3(-12.0, 1.0f, 4.0f),
        EO_COLOR3_HEX(0xFB8500),
        EO_COLOR3_HEX(0xFB8500),
        EO_COLOR3_HEX(0xFFF0D6),
        glm::vec3(0.0f, 4.0f, -20.0f),
        EO_COLOR3_HEX(0xDDE392),
        EO_COLOR3_HEX(0xDDE392),
        EO_COLOR3_HEX(0xF0F2CE),
    };
    constexpr int point_light_stride = 4;
    int point_light_count = static_cast<int>(point_light_data.size() / point_light_stride);
    _shader->set_uniform("u_point_light_count", point_light_count);
    for (int i = 0; i < point_light_count; i += 1)
    {
        PointLight{*_shader,
                   std::format("u_point_lights[{}]", i),
                   point_light_data[i * point_light_stride + 1],
                   point_light_data[i * point_light_stride + 2],
                   point_light_data[i * point_light_stride + 3],
                   point_light_data[i * point_light_stride],
                   1.0f,
                   0.045f,
                   0.0075f};
    }

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

    Shader::unuse_all();

    std::vector<std::shared_ptr<Texture>> mat_dirt_textures{
        std::make_shared<Texture>(Texture::Type::diffuse, "resources/textures/dirt.png"),
        Texture::no_specular(),
    };
    _mat_dirt = std::make_shared<Material>(mat_dirt_textures, 25.0f);
    _ground = std::make_unique<Mesh>(Primitive::quad(), _mat_dirt);

    std::vector<std::shared_ptr<Texture>> mat_box_textures{
        std::make_shared<Texture>(Texture::Type::diffuse,
                                  "resources/textures/container2_diffuse.png"),
        std::make_shared<Texture>(Texture::Type::specular,
                                  "resources/textures/container2_specular.png"),
    };
    _mat_box = std::make_shared<Material>(mat_box_textures, 400.0f);
    _cube = std::make_unique<Mesh>(Primitive::cube(), _mat_box);

    for (size_t i = 0; i < _cube_positions.size(); i += 1)
    {
        _cube_positions[i] = {
            random_float(-90.0f, 90.0f),
            random_float(2.0f, 20.0f),
            random_float(-90.0f, 90.0f),
        };
    }

    register_mouse_look(*_camera);
}

PhongLab::~PhongLab() {}

void PhongLab::on_update()
{
    toggle_movement();
    strafe(*_camera);
}

void PhongLab::on_render()
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

    Transform ground_transform{};
    ground_transform.scale(glm::vec3(200.0f, 1.0f, 200.0f));
    _shader->set_uniform("u_model", ground_transform.matrix());
    _shader->set_uniform("u_texture_scale", 0.02f);
    _mat_dirt->bind(*_shader);
    _ground->draw();
    _mat_dirt->unbind(*_shader);

    Transform cube_transform{};
    cube_transform.rotation(Time::current_time() * glm::radians(50.0f), {0.5f, 1.0f, 0.0f});
    _shader->set_uniform("u_texture_scale", 1.0f);
    _mat_box->bind(*_shader);
    for (glm::vec3 position : _cube_positions)
    {
        cube_transform.position(position);
        _shader->set_uniform("u_model", cube_transform.matrix());
        _cube->draw();
    }
    _mat_box->unbind(*_shader);
}

} // namespace eo

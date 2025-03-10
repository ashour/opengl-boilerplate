#include "lib/color.h"
#include "lib/random.h"
#include "objects/primitive.h"
#include "registry/lab_registry.h"
#include "rendering/transform.h"
#include "shadow_mapping_lab.h"
#include "system/time.h"

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab("15. Shadow mapping",
                                  [](eo::Window& window) -> eo::Lab*
                                  { return new eo::ShadowMappingLab(window); });
    return true;
}();
} // namespace

namespace eo
{

ShadowMappingLab::ShadowMappingLab(const Window& window) : Lab(window)
{
    _window.set_clear_color({254.0f / 255.0f, 249.0f / 255.0f, 186.0f / 255.0f, 1.0f});

    _shader = std::make_shared<Shader>("resources/shaders/shadow_mapping_scene.vert",
                                       "resources/shaders/shadow_mapping_scene.frag");
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
                                                            EO_COLOR3_HEX(0xB95900),
                                                            EO_COLOR3_HEX(0xFFD000),
                                                            EO_COLOR3_HEX(0xFFEA00));

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

ShadowMappingLab::~ShadowMappingLab() {}

void ShadowMappingLab::on_update()
{
    toggle_movement();
    strafe_and_fly(*_camera);
}

void ShadowMappingLab::on_render()
{
    _shader->use();
    _shader->set_uniform(_u_view_matrix, _camera->view());
    _shader->set_uniform(_u_view_position, _camera->position());

    if (_rotate_light)
    {
        constexpr float light_orbit_radius = 50.0f;
        float light_y_tilt = glm::tan(glm::radians(-30.0f));
        float light_angle =
            std::fmod(glm::radians(Time::current_time() * 20.0f), glm::two_pi<float>());
        _directional_light->set_direction(glm::vec3(light_orbit_radius * glm::cos(light_angle),
                                                    light_orbit_radius * light_y_tilt,
                                                    light_orbit_radius * glm::sin(light_angle)));
    }
    else
    {
        _directional_light->set_direction({-10.0f, -3.0f, 0.0f});
    }

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

void ShadowMappingLab::on_ui_render(UI& ui)
{
    movement_help_ui(ui);

    ui.begin_window("Rotate light");
    ImGui::Checkbox("Rotate light", &_rotate_light);
    ui.end_window();
}

} // namespace eo

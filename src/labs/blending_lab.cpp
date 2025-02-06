#include "blending_lab.h"
#include "config.h"
#include "lib/random.h"
#include "objects/primitive.h"
#include "rendering/transform.h"
#include "system/input.h"
#include "system/time.h"

namespace eo
{

BlendingLab::BlendingLab(const Window& window) : Lab(window)
{
    _window.set_clear_color(SCENE_CLEAR_COLOR);

    _shader = std::make_shared<Shader>("resources/shaders/unlit_texture.vert",
                                       "resources/shaders/unlit_texture.frag");
    _shader->build();

    _shader->use();
    _u_view_matrix = _shader->uniform_location_for("u_view");

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _shader->set_uniform("u_projection", _camera->projection());

    Shader::unuse_all();

    std::vector<std::shared_ptr<Texture>> mat_dirt_textures{
        std::make_shared<Texture>(Texture::Type::diffuse, "resources/textures/dirt.png"),
        Texture::no_specular(),
    };
    _mat_dirt = std::make_shared<Material>(mat_dirt_textures, 25.0f);
    _plane = std::make_unique<Mesh>(Primitive::quad(), _mat_dirt);

    std::vector<std::shared_ptr<Texture>> mat_box_textures{
        std::make_shared<Texture>(Texture::Type::diffuse,
                                  "resources/textures/container2_diffuse.png"),
        Texture::no_specular()};
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

    Input::register_mouse_move_handler(
        [this](auto current_mouse_position, auto last_mouse_position)
        { _camera->look(current_mouse_position, last_mouse_position); });
}

BlendingLab::~BlendingLab() {}

void BlendingLab::on_update()
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

void BlendingLab::on_render()
{
    _shader->use();
    _shader->set_uniform(_u_view_matrix, _camera->view());

    Transform plane_transform{};
    plane_transform.scale(glm::vec3(200.0f, 1.0f, 200.0f));
    _shader->set_uniform("u_model", plane_transform.matrix());
    _shader->set_uniform("u_texture_scale", 0.02f);
    _mat_dirt->bind(*_shader);
    _plane->draw();
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

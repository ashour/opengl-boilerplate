#include "gsl_visualize_normals.h"
#include "rendering/transform.h"

namespace eo
{
Gsl_VisualizeNormals::Gsl_VisualizeNormals(const Window& window) : Lab(window)
{
    _window.set_clear_color(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _backpack_shader = std::make_unique<Shader>("resources/shaders/unlit_texture.vert",
                                                "resources/shaders/unlit_texture.frag");
    _backpack_shader->build();
    _backpack_shader->use();
    _backpack_shader->set_uniform("u_projection", _camera->projection());

    _visualize_normals_shader =
        std::make_unique<Shader>("resources/shaders/visualize_normals.vert",
                                 "resources/shaders/visualize_normals.frag",
                                 "resources/shaders/visualize_normals.geom");
    _visualize_normals_shader->build();
    _visualize_normals_shader->use();
    _visualize_normals_shader->set_uniform("u_projection", _camera->projection());

    _backpack = std::make_unique<Model>("resources/models/backpack/backpack.obj");

    register_mouse_look(*_camera);
}

Gsl_VisualizeNormals::~Gsl_VisualizeNormals() {}

void Gsl_VisualizeNormals::on_update()
{
    toggle_movement();
    strafe_and_fly(*_camera);
}

void Gsl_VisualizeNormals::on_render()
{
    Transform backpack_transform{};
    backpack_transform.position({0.0f, 4.0f, 10.0f});
    backpack_transform.scale(glm::vec3{2.0f});

    _backpack_shader->use();
    _backpack_shader->set_uniform("u_view", _camera->view());
    _backpack_shader->set_uniform("u_model", backpack_transform.matrix());
    _backpack_shader->set_uniform("u_texture_scale", 1.0f);
    _backpack->draw(*_backpack_shader);
    _backpack->unbind_materials(*_backpack_shader);

    _visualize_normals_shader->use();
    _visualize_normals_shader->set_uniform("u_view", _camera->view());
    _visualize_normals_shader->set_uniform("u_model", backpack_transform.matrix());
    _backpack->draw(*_visualize_normals_shader, false);
}

void Gsl_VisualizeNormals::on_ui_render(UI& ui) { movement_help_ui(ui); }

} // namespace eo

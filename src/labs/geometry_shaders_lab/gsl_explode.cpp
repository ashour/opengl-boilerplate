#include "gsl_explode.h"
#include "rendering/transform.h"
#include "system/time.h"

namespace eo
{

Gsl_Explode::Gsl_Explode(const Window& window) : Lab(window)
{
    _window.set_clear_color(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _explode_shader = std::make_unique<Shader>("resources/shaders/explode.vert",
                                               "resources/shaders/explode.frag",
                                               "resources/shaders/explode.geom");
    _explode_shader->build();
    _explode_shader->use();

    _explode_shader->set_uniform("u_projection", _camera->projection());
    _explode_shader->set_uniform("u_view", _camera->view());

    _backpack = std::make_unique<Model>("resources/models/backpack/backpack.obj");
}

Gsl_Explode::~Gsl_Explode() {}

void Gsl_Explode::on_render()
{
    Transform backpack_transform{};
    backpack_transform.position({0.0f, 4.0f, 10.0f});
    backpack_transform.scale(glm::vec3{2.0f});
    _explode_shader->set_uniform("u_model", backpack_transform.matrix());
    _explode_shader->set_uniform("u_texture_scale", 1.0f);
    _explode_shader->set_uniform("u_time", Time::current_time());
    _backpack->draw(*_explode_shader);
    _backpack->unbind_materials(*_explode_shader);
}

} // namespace eo

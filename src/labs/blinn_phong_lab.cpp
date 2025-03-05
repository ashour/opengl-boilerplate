#include "blinn_phong_lab.h"
#include "config.h"
#include "registry/lab_registry.h"
#include "system/metrics.h"

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab("14. Blinn-Phong shading",
                                  [](eo::Window& window) -> eo::Lab*
                                  { return new eo::BlinnPhongLab(window); });
    return true;
}();
} // namespace

namespace eo
{

BlinnPhongLab::BlinnPhongLab(const Window& window)
    : Lab(window),
      _ground_texture{Texture::Type::diffuse, "resources/textures/wood.png", Texture::Wrap::repeat}
{
    _window.set_clear_color(SCENE_CLEAR_COLOR);

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _shader = std::make_unique<Shader>("resources/shaders/blinn_phong.vert",
                                       "resources/shaders/blinn_phong.frag");
    _shader->build();
    _shader->use();

    _shader->set_uniform("u_projection", _camera->projection());
    _shader->set_uniform("u_light_position", glm::vec3{0.0f});

    // clang-format off
    float ground_vertices[] = {
        -10.0f, -5.0f,  10.0f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
         10.0f, -5.0f,  10.0f, 0.0f, 1.0f, 0.0f, 10.0f,  0.0f,
        -10.0f, -5.0f, -10.0f, 0.0f, 1.0f, 0.0f,  0.0f, 10.0f,

         10.0f, -5.0f,  10.0f, 0.0f, 1.0f, 0.0f, 10.0f,  0.0f,
         10.0f, -5.0f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f,
        -10.0f, -5.0f, -10.0f, 0.0f, 1.0f, 0.0f,  0.0f, 10.0f,
    };
    // clang-format on

    _ground_vertex_array = std::make_unique<VertexArray>(sizeof(ground_vertices), ground_vertices);
    _ground_vertex_array->attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    _ground_vertex_array->attribute_pointer(
        1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    _ground_vertex_array->attribute_pointer(
        2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    gldc(glActiveTexture(GL_TEXTURE0));
    gldc(glBindTexture(GL_TEXTURE_2D, _ground_texture.id()));

    register_mouse_look(*_camera);
}

BlinnPhongLab::~BlinnPhongLab() {}

void BlinnPhongLab::on_update()
{
    toggle_movement();
    strafe_and_fly(*_camera);
}

void BlinnPhongLab::on_render()
{
    _shader->set_uniform("u_view", _camera->view());
    _shader->set_uniform("u_view_position", _camera->position());
    _shader->set_uniform("u_use_blinn", _use_blinn);

    gldc(glDrawArrays(GL_TRIANGLES, 0, 6));

    if (SHOW_UI_METRICS)
    {
        Metrics::add_to_drawn_vertex_count(3 * 6);
    }
}

void BlinnPhongLab::on_ui_render(UI& ui)
{
    movement_help_ui(ui);

    ui.begin_window("Blinn-Phong shading");
    if (ui.radio_button("Phong", !_use_blinn))
    {
        _use_blinn = false;
    }
    if (ui.radio_button("Blinn-Phong", _use_blinn))
    {
        _use_blinn = true;
    }
    ui.end_window();
}

} // namespace eo

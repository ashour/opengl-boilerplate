#include "config.h"
#include "lib/random.h"
#include "objects/primitive.h"
#include "registry/lab_registry.h"
#include "rendering/transform.h"
#include "stencil_testing_lab.h"
#include "system/time.h"

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab("Stencil testing",
                                  [](eo::Window& window) -> eo::Lab*
                                  { return new eo::StencilTestingLab(window); });
    return true;
}();
} // namespace

namespace eo
{

StencilTestingLab::StencilTestingLab(const Window& window) : Lab(window)
{
    _window.set_clear_color(SCENE_CLEAR_COLOR);

    _render_shader = std::make_shared<Shader>("resources/shaders/unlit_texture.vert",
                                              "resources/shaders/unlit_texture.frag");
    _render_shader->build();

    _stencil_shader = std::make_shared<Shader>("resources/shaders/unlit_texture.vert",
                                               "resources/shaders/single_color.frag");
    _stencil_shader->build();

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _render_shader->use();
    _render_shader->set_uniform("u_projection", _camera->projection());

    _stencil_shader->use();
    _stencil_shader->set_uniform("u_projection", _camera->projection());

    Shader::unuse_all();

    std::vector<std::shared_ptr<Texture>> mat_dirt_textures{
        std::make_shared<Texture>(Texture::Type::diffuse, "resources/textures/dirt.png"),
        Texture::no_specular(),
    };
    _mat_dirt = std::make_shared<Material>(mat_dirt_textures, 0.0f);
    _ground = std::make_unique<Mesh>(Primitive::quad(), _mat_dirt);

    std::vector<std::shared_ptr<Texture>> mat_box_textures{
        std::make_shared<Texture>(Texture::Type::diffuse,
                                  "resources/textures/container2_diffuse.png"),
        Texture::no_specular(),
    };
    _mat_box = std::make_shared<Material>(mat_box_textures, 0.0f);
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

void StencilTestingLab::on_update()
{
    toggle_movement();
    strafe(*_camera);
}

void StencilTestingLab::on_render()
{
    gldc(glEnable(GL_DEPTH_TEST));
    gldc(glEnable(GL_STENCIL_TEST));
    gldc(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));

    gldc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    gldc(glStencilMask(0x00));
    _render_shader->use();
    _render_shader->set_uniform("u_texture_scale", 0.05f);
    _mat_dirt->bind(*_render_shader);
    Transform ground_transform{};
    ground_transform.scale(glm::vec3(200.0f, 1.0f, 200.0f));
    _render_shader->set_uniform("u_model", ground_transform.matrix());
    _ground->draw();
    _mat_dirt->unbind(*_render_shader);

    gldc(glStencilFunc(GL_ALWAYS, 1, 0xFF));
    gldc(glStencilMask(0xFF));
    _render_shader->use();
    _render_shader->set_uniform("u_view", _camera->view());
    _render_shader->set_uniform("u_texture_scale", 1.0f);
    _mat_box->bind(*_render_shader);
    Transform cube_transform{};
    cube_transform.rotation(Time::current_time() * glm::radians(50.0f), {0.5f, 1.0f, 0.0f});
    for (glm::vec3 position : _cube_positions)
    {
        cube_transform.position(position);
        _render_shader->set_uniform("u_model", cube_transform.matrix());
        _cube->draw();
    }
    _mat_box->unbind(*_render_shader);

    gldc(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
    gldc(glStencilMask(0x00));
    gldc(glDisable(GL_DEPTH_TEST));
    _stencil_shader->use();
    _stencil_shader->set_uniform("u_view", _camera->view());
    cube_transform.scale(glm::vec3(1.1f));
    for (glm::vec3 position : _cube_positions)
    {
        cube_transform.position(position);
        _stencil_shader->set_uniform("u_model", cube_transform.matrix());
        _cube->draw();
    }
    gldc(glStencilMask(0xFF));
    gldc(glStencilFunc(GL_ALWAYS, 1, 0xFF));
    gldc(glEnable(GL_DEPTH_TEST));
}

void StencilTestingLab::on_ui_render(UI& ui) { movement_help_ui(ui); }

} // namespace eo

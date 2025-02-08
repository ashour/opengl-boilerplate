#include "blending_lab.h"
#include "config.h"
#include "lib/random.h"
#include "objects/primitive.h"
#include "registry/lab_registry.h"
#include "rendering/transform.h"
#include "system/time.h"
#include <map>

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab(
        "Blending", [](eo::Window& window) -> eo::Lab* { return new eo::BlendingLab(window); });
    return true;
}();
} // namespace

namespace eo
{

BlendingLab::BlendingLab(const Window& window) : Lab(window)
{
    _window.set_clear_color(SCENE_CLEAR_COLOR);

    _unlit_tex_shader = std::make_shared<Shader>("resources/shaders/unlit_texture.vert",
                                                 "resources/shaders/unlit_texture.frag");
    _unlit_tex_shader->build();

    _discard_transparent_tex_shader =
        std::make_shared<Shader>("resources/shaders/unlit_texture.vert",
                                 "resources/shaders/discard_transparent_texture.frag");
    _discard_transparent_tex_shader->build();

    _unlit_alpha_texture = std::make_shared<Shader>("resources/shaders/unlit_texture.vert",
                                                    "resources/shaders/unlit_alpha_texture.frag");
    _unlit_alpha_texture->build();

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _unlit_tex_shader->use();
    _u_view_matrix = _unlit_tex_shader->uniform_location_for("u_view");
    _unlit_tex_shader->set_uniform("u_projection", _camera->projection());

    _discard_transparent_tex_shader->use();
    _u_view_matrix = _discard_transparent_tex_shader->uniform_location_for("u_view");
    _discard_transparent_tex_shader->set_uniform("u_projection", _camera->projection());

    _unlit_alpha_texture->use();
    _u_view_matrix = _unlit_alpha_texture->uniform_location_for("u_view");
    _unlit_alpha_texture->set_uniform("u_projection", _camera->projection());

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

    std::vector<std::shared_ptr<Texture>> mat_grass_textures{
        std::make_shared<Texture>(
            Texture::Type::diffuse, "resources/textures/grass.png", Texture::Wrap::clamp_to_edge),
        Texture::no_specular(),
    };
    _mat_grass = std::make_shared<Material>(mat_grass_textures);
    _grass = std::make_unique<Mesh>(Primitive::quad(), _mat_grass);

    std::vector<std::shared_ptr<Texture>> mat_window_textures{
        std::make_shared<Texture>(Texture::Type::diffuse,
                                  "resources/textures/blending_transparent_window.png",
                                  Texture::Wrap::clamp_to_edge),
        Texture::no_specular(),
    };
    _mat_window = std::make_shared<Material>(mat_window_textures);
    _transparent_window = std::make_unique<Mesh>(Primitive::quad(), _mat_window);

    register_mouse_look_on_hold_rmb(*_camera);
}

BlendingLab::~BlendingLab() {}

void BlendingLab::on_update() { wasd_move_on_hold_rmb(*_camera); }

void BlendingLab::on_render()
{
    _unlit_tex_shader->use();
    _unlit_tex_shader->set_uniform(_u_view_matrix, _camera->view());

    Transform ground_transform{};
    ground_transform.scale(glm::vec3(200.0f, 1.0f, 200.0f));
    _unlit_tex_shader->set_uniform("u_model", ground_transform.matrix());
    _unlit_tex_shader->set_uniform("u_texture_scale", 0.02f);
    _mat_dirt->bind(*_unlit_tex_shader);
    _ground->draw();
    _mat_dirt->unbind(*_unlit_tex_shader);

    Transform cube_transform{};
    cube_transform.rotation(Time::current_time() * glm::radians(50.0f), {0.5f, 1.0f, 0.0f});
    _unlit_tex_shader->set_uniform("u_texture_scale", 1.0f);
    _mat_box->bind(*_unlit_tex_shader);
    for (glm::vec3 position : _cube_positions)
    {
        cube_transform.position(position);
        _unlit_tex_shader->set_uniform("u_model", cube_transform.matrix());
        _cube->draw();
    }
    _mat_box->unbind(*_unlit_tex_shader);

    gldc(glDisable(GL_CULL_FACE));
    _discard_transparent_tex_shader->use();
    _discard_transparent_tex_shader->set_uniform(_u_view_matrix, _camera->view());
    Transform grass_transform{};
    grass_transform.rotation(glm::radians(90.0f), {1, 0, 0});
    grass_transform.position({0, 1.0f, 0});
    grass_transform.scale(glm::vec3(2.0f));
    _discard_transparent_tex_shader->set_uniform("u_model", grass_transform.matrix());
    _discard_transparent_tex_shader->set_uniform("u_texture_scale", 1.0f);
    _mat_grass->bind(*_discard_transparent_tex_shader);
    _grass->draw();
    _mat_grass->unbind(*_discard_transparent_tex_shader);

    gldc(glEnable(GL_BLEND));
    gldc(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    _unlit_alpha_texture->use();
    _unlit_alpha_texture->set_uniform(_u_view_matrix, _camera->view());
    _unlit_alpha_texture->set_uniform("u_texture_scale", 1.0f);
    Transform window_transform{};
    window_transform.rotation(glm::radians(90.0f), {1, 0, 0});

    std::map<float, glm::vec3> sorted_windows;
    for (glm::vec3 window_position : _window_positions)
    {
        float distance = glm::length(_camera->position() - window_position);
        sorted_windows[distance] = window_position;
    }
    for (auto it = sorted_windows.rbegin(); it != sorted_windows.rend(); it++)
    {
        window_transform.position(it->second);
        _unlit_alpha_texture->set_uniform("u_model", window_transform.matrix());
        _mat_window->bind(*_unlit_alpha_texture);
        _transparent_window->draw();
        _mat_window->unbind(*_unlit_alpha_texture);
    }
    gldc(glDisable(GL_BLEND));
    gldc(glEnable(GL_CULL_FACE));
}

} // namespace eo

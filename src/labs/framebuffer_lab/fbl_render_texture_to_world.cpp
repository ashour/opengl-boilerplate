#include "fbl_render_texture_to_world.h"
#include "lib/random.h"
#include "objects/primitive.h"
#include "rendering/transform.h"
#include "system/time.h"

namespace eo
{

Fbl_RenderTextureToWorld::Fbl_RenderTextureToWorld(const Window& window) : Lab(window)
{
    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _scene_shader = std::make_shared<Shader>("resources/shaders/unlit_texture.vert",
                                             "resources/shaders/unlit_texture.frag");
    _scene_shader->build();
    _scene_shader->use();
    _scene_shader->set_uniform("u_projection", _camera->projection());

    _quad_shader = std::make_shared<Shader>("resources/shaders/unlit_texture.vert",
                                            "resources/shaders/unlit_texture.frag");
    _quad_shader->build();
    _quad_shader->use();
    _quad_shader->set_uniform("u_projection", _camera->projection());

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
        Texture::no_specular()};
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

    _quad = std::make_unique<Mesh>(Primitive::quad(), nullptr);

    _framebuffer = std::make_unique<Framebuffer>(_window.buffer_width(), _window.buffer_height());

    register_mouse_look(*_camera);
}

Fbl_RenderTextureToWorld::~Fbl_RenderTextureToWorld() {}

void Fbl_RenderTextureToWorld::on_update()
{
    toggle_movement();
    strafe_and_fly(*_camera);
}

void Fbl_RenderTextureToWorld::on_render()
{
    _framebuffer->bind();
    gldc(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    gldc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    gldc(glEnable(GL_DEPTH_TEST));
    render_scene();

    _framebuffer->unbind();
    render_scene();

    Transform quad_transform{};
    quad_transform.scale({16.0f, 1.0f, 9.0f});
    quad_transform.rotation(glm::radians(90.0f), {1.0f, 0.0f, 0.0f});
    quad_transform.position({0.0f, 3.5f, -10.0f});

    _quad_shader->use();

    gldc(glActiveTexture(GL_TEXTURE0));
    _framebuffer->bind_color_texture();
    _quad_shader->set_uniform("u_material.diffuse_1", 0);

    _quad_shader->set_uniform("u_view", _camera->view());
    _quad_shader->set_uniform("u_model", quad_transform.matrix());
    _quad->draw();
}

void Fbl_RenderTextureToWorld::on_ui_render(UI& ui) { movement_help_ui(ui); }

void Fbl_RenderTextureToWorld::render_scene()
{
    _scene_shader->use();
    _scene_shader->set_uniform("u_view", _camera->view());

    Transform ground_transform{};
    ground_transform.scale(glm::vec3(200.0f, 1.0f, 200.0f));
    _scene_shader->set_uniform("u_model", ground_transform.matrix());
    _scene_shader->set_uniform("u_texture_scale", 0.02f);
    _mat_dirt->bind(*_scene_shader);
    _ground->draw();
    _mat_dirt->unbind(*_scene_shader);

    Transform cube_transform{};
    cube_transform.rotation(Time::current_time() * glm::radians(50.0f), {0.5f, 1.0f, 0.0f});
    _scene_shader->set_uniform("u_texture_scale", 1.0f);
    _mat_box->bind(*_scene_shader);
    for (glm::vec3 position : _cube_positions)
    {
        cube_transform.position(position);
        _scene_shader->set_uniform("u_model", cube_transform.matrix());
        _cube->draw();
    }
    _mat_box->unbind(*_scene_shader);
}

} // namespace eo

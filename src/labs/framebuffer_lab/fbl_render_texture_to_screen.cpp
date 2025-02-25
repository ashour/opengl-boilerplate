#include "fbl_render_texture_to_screen.h"
#include "lib/random.h"
#include "objects/primitive.h"
#include "rendering/transform.h"
#include "system/time.h"

namespace eo
{

Fbl_RenderTextureToScreen::Fbl_RenderTextureToScreen(
    const Window& window,
    const std::string& screen_fragment_shader_file_path,
    const bool upload_screen_dimensions,
    const std::vector<float>* kernel)
    : Lab(window)
{
    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _scene_shader = std::make_shared<Shader>("resources/shaders/unlit_texture.vert",
                                             "resources/shaders/unlit_texture.frag");
    _scene_shader->build();
    _scene_shader->use();
    _scene_shader->set_uniform("u_projection", _camera->projection());

    _quad_shader =
        std::make_shared<Shader>("resources/shaders/screen.vert", screen_fragment_shader_file_path);
    _quad_shader->build();
    _quad_shader->use();
    if (upload_screen_dimensions)
    {
        _quad_shader->set_uniform("u_screen_dimensions",
                                  glm::vec2{static_cast<float>(_window.buffer_width()),
                                            static_cast<float>(_window.buffer_height())});
    }
    if (kernel)
    {
        _quad_shader->set_uniform("u_kernel", *kernel);
    }

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

    std::vector<Vertex> quad_vertices = {
        {{-1.0f, 1.0f, 0.0f}, glm::vec3{0.0f}, {0.0f, 1.0f}},
        {{-1.0f, -1.0f, 0.0f}, glm::vec3{0.0f}, {0.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, glm::vec3{0.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, glm::vec3{0.0f}, {1.0f, 1.0f}},
    };
    std::vector<unsigned int> quad_indices = {0, 1, 2, 2, 3, 0};
    _quad = std::make_unique<Mesh>(quad_vertices, quad_indices, nullptr);

    gldc(glGenFramebuffers(1, &_fbo));
    gldc(glBindFramebuffer(GL_FRAMEBUFFER, _fbo));

    gldc(glGenTextures(1, &_tex_color_buffer));
    gldc(glBindTexture(GL_TEXTURE_2D, _tex_color_buffer));
    gldc(glTexImage2D(GL_TEXTURE_2D,
                      0,
                      GL_RGB,
                      _window.buffer_width(),
                      _window.buffer_height(),
                      0,
                      GL_RGB,
                      GL_UNSIGNED_BYTE,
                      NULL));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    gldc(glBindTexture(GL_TEXTURE_2D, 0));
    gldc(glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex_color_buffer, 0));

    gldc(glGenRenderbuffers(1, &_rbo));
    gldc(glBindRenderbuffer(GL_RENDERBUFFER, _rbo));
    gldc(glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _window.buffer_width(), _window.buffer_height()));
    gldc(glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        EO_LOG_ERROR("[Framebuffer Error] Framebuffer is not complete!");
    }

    // gldc(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

    register_mouse_look(*_camera);
}

Fbl_RenderTextureToScreen::~Fbl_RenderTextureToScreen()
{
    gldc(glDeleteFramebuffers(1, &_fbo));
    gldc(glDeleteRenderbuffers(1, &_rbo));
    gldc(glDeleteTextures(1, &_tex_color_buffer));
}

void Fbl_RenderTextureToScreen::on_update()
{
    toggle_movement();
    strafe(*_camera);
}

void Fbl_RenderTextureToScreen::on_render()
{
    gldc(glBindFramebuffer(GL_FRAMEBUFFER, _fbo));
    gldc(glEnable(GL_DEPTH_TEST));
    gldc(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    gldc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    render_scene();

    gldc(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    gldc(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
    gldc(glDisable(GL_DEPTH_TEST));
    gldc(glClear(GL_COLOR_BUFFER_BIT));

    _quad_shader->use();
    gldc(glActiveTexture(GL_TEXTURE0));
    gldc(glBindTexture(GL_TEXTURE_2D, _tex_color_buffer));
    _quad_shader->set_uniform("u_material.diffuse_1", 0);
    _quad->draw();
}

void Fbl_RenderTextureToScreen::on_ui_render(UI& ui) { movement_help_ui(ui); }

void Fbl_RenderTextureToScreen::render_scene()
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

#include "antialiasing_lab.h"
#include "config.h"
#include "objects/primitive.h"
#include "registry/lab_registry.h"
#include "rendering/transform.h"

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab("Antialiasing",
                                  [](eo::Window& window) -> eo::Lab*
                                  { return new eo::AntialiasingLab(window); });
    return true;
}();
} // namespace

namespace eo
{

AntialiasingLab::AntialiasingLab(const Window& window) : Lab(window)
{
    glEnable(GL_MULTISAMPLE);
    _window.set_clear_color(SCENE_CLEAR_COLOR);

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _scene_shader = std::make_shared<Shader>("resources/shaders/antialiasing_scene.vert",
                                             "resources/shaders/antialiasing_scene.frag");
    _scene_shader->build();
    _scene_shader->use();
    _scene_shader->set_uniform("u_projection", _camera->projection());

    _screen_shader = std::make_shared<Shader>("resources/shaders/antialiasing_screen.vert",
                                              "resources/shaders/antialiasing_screen.frag");
    _screen_shader->build();
    _screen_shader->use();
    _screen_shader->set_uniform("u_screen_texture", 0);

    Shader::unuse_all();

    _cube = std::make_unique<Mesh>(Primitive::cube());

    std::vector<Vertex> quad_vertices = {
        {{-1.0f, 1.0f, 0.0f}, glm::vec3{0.0f}, {0.0f, 1.0f}},
        {{-1.0f, -1.0f, 0.0f}, glm::vec3{0.0f}, {0.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, glm::vec3{0.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, glm::vec3{0.0f}, {1.0f, 1.0f}},
    };
    std::vector<unsigned int> quad_indices = {0, 1, 2, 2, 3, 0};
    _screen_quad = std::make_unique<Mesh>(quad_vertices, quad_indices, nullptr);

    // Configure MSAA framebuffer
    gldc(glGenFramebuffers(1, &_multisampled_fbo));
    gldc(glBindFramebuffer(GL_FRAMEBUFFER, _multisampled_fbo));
    // Create a multisampled color attachment texture
    gldc(glGenTextures(1, &_color_buffer_multisampled_texture));
    gldc(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _color_buffer_multisampled_texture));
    gldc(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                                 MULTISAMPLE_COUNT,
                                 GL_RGB,
                                 _window.buffer_width(),
                                 _window.buffer_height(),
                                 GL_TRUE));
    gldc(glBindTexture(GL_TEXTURE_2D, 0));
    gldc(glFramebufferTexture2D(GL_FRAMEBUFFER,
                                GL_COLOR_ATTACHMENT0,
                                GL_TEXTURE_2D_MULTISAMPLE,
                                _color_buffer_multisampled_texture,
                                0));
    // Create a multisampled renderbuffer object for depth and stencil attachments
    gldc(glGenRenderbuffers(1, &_rbo));
    gldc(glBindRenderbuffer(GL_RENDERBUFFER, _rbo));
    gldc(glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                                          MULTISAMPLE_COUNT,
                                          GL_DEPTH24_STENCIL8,
                                          _window.buffer_width(),
                                          _window.buffer_height()));
    gldc(glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        EO_LOG_ERROR("Framebuffer is not complete!");
    }
    gldc(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    // Configure post-processing framebuffer
    gldc(glGenFramebuffers(1, &_intermediate_fbo));
    gldc(glBindFramebuffer(GL_FRAMEBUFFER, _intermediate_fbo));
    // Create a color attachment texture
    gldc(glGenTextures(1, &_screen_texture));
    gldc(glBindTexture(GL_TEXTURE_2D, _screen_texture));
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
    gldc(glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _screen_texture, 0));
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        EO_LOG_ERROR("Framebuffer is not complete!");
    }
    gldc(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    register_mouse_look(*_camera);
}

AntialiasingLab::~AntialiasingLab()
{
    glDeleteFramebuffers(1, &_multisampled_fbo);
    glDeleteFramebuffers(1, &_intermediate_fbo);
    glDeleteTextures(1, &_color_buffer_multisampled_texture);
    glDeleteTextures(1, &_screen_texture);
    glDeleteRenderbuffers(1, &_rbo);
}

void AntialiasingLab::on_update()
{
    toggle_movement();
    strafe_and_fly(*_camera);
}

void AntialiasingLab::on_render()
{
    gldc(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    gldc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 1. Draw scene as normal in multisampled buffer
    gldc(glBindFramebuffer(GL_FRAMEBUFFER, _multisampled_fbo));
    gldc(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    gldc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    gldc(glEnable(GL_DEPTH_TEST));

    _scene_shader->use();
    _scene_shader->set_uniform("u_view", _camera->view());

    Transform cube_transform{};
    cube_transform.scale(glm::vec3(10.0f));
    cube_transform.position({0.0f, 5.1f, 0.0f});
    cube_transform.rotation(glm::radians(20.0f), glm::vec3{0.6f, 0.2f, 0.2f});
    _scene_shader->set_uniform("u_model", cube_transform.matrix());
    _cube->draw();

    // 2. Blit multisampled buffer to normal colorbuffer of intermediate fbo.
    //    Image is stored in screen texture.
    gldc(glBindFramebuffer(GL_READ_FRAMEBUFFER, _multisampled_fbo));
    gldc(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _intermediate_fbo));
    gldc(glBlitFramebuffer(0,
                           0,
                           _window.buffer_width(),
                           _window.buffer_height(),
                           0,
                           0,
                           _window.buffer_width(),
                           _window.buffer_height(),
                           GL_COLOR_BUFFER_BIT,
                           GL_NEAREST));

    // 3. Render screen quad with scene's visuals as its texture image.
    gldc(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    gldc(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
    gldc(glClear(GL_COLOR_BUFFER_BIT));
    gldc(glDisable(GL_DEPTH_TEST));

    _screen_shader->use();
    gldc(glActiveTexture(GL_TEXTURE0));
    gldc(glBindTexture(GL_TEXTURE_2D, _screen_texture));
    _screen_quad->draw(*_screen_shader);
}

void AntialiasingLab::on_ui_render(UI& ui)
{
    movement_help_ui(ui);
    ui.begin_window("Antialiasing");
    ui.text("Make sure to set MULTISAMPLE_COUNT to \n4 in config.h and re-run the app.");
    ui.end_window();
}

} // namespace eo

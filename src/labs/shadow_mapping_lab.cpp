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

    _scene_shader = std::make_shared<Shader>("resources/shaders/shadow_mapping_scene.vert",
                                             "resources/shaders/shadow_mapping_scene.frag");
    _scene_shader->build();

    _scene_shader->use();
    _u_view_matrix = _scene_shader->uniform_location_for("u_view");
    _u_view_position = _scene_shader->uniform_location_for("u_view_position");

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _scene_shader->set_uniform("u_projection", _camera->projection());

    _directional_light = std::make_unique<DirectionalLight>(*_scene_shader,
                                                            "u_directional_light",
                                                            glm::vec3(-1.5f, -1.0f, 0.0f),
                                                            EO_COLOR3_HEX(0xB95900),
                                                            EO_COLOR3_HEX(0xFFD000),
                                                            EO_COLOR3_HEX(0xFFEA00));

    _scene_shader->set_uniform("u_tex_shadow_map", 4);
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

    gldc(glGenFramebuffers(1, &_depth_map_fbo));

    gldc(glGenTextures(1, &_depth_map_tex));
    gldc(glBindTexture(GL_TEXTURE_2D, _depth_map_tex));
    gldc(glTexImage2D(GL_TEXTURE_2D,
                      0,
                      GL_DEPTH_COMPONENT,
                      SHADOW_WIDTH,
                      SHADOW_HEIGHT,
                      0,
                      GL_DEPTH_COMPONENT,
                      GL_FLOAT,
                      NULL));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
    float border_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    gldc(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color));

    gldc(glBindFramebuffer(GL_FRAMEBUFFER, _depth_map_fbo));
    gldc(glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_map_tex, 0));
    gldc(glDrawBuffer(GL_NONE));
    gldc(glReadBuffer(GL_NONE));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        EO_LOG_ERROR("[Framebuffer Error] Framebuffer is not complete!");
    }

    gldc(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    _simple_depth_shader = std::make_unique<Shader>("resources/shaders/shadow_map_depth.vert",
                                                    "resources/shaders/shadow_map_depth.frag");
    _simple_depth_shader->build();

    _debug_quad = std::make_unique<Mesh>(Primitive::quad());
    Transform debug_quad_transform;
    debug_quad_transform.rotation(glm::radians(90.0f), {1.0f, 0.0f, 0.0f});
    debug_quad_transform.scale(glm::vec3{2.0f});
    _debug_quad_shader = std::make_unique<Shader>("resources/shaders/debug_depth_quad.vert",
                                                  "resources/shaders/debug_depth_quad.frag");
    _debug_quad_shader->build();
    _debug_quad_shader->use();
    _debug_quad_shader->set_uniform("u_depth_map", 0);
    _debug_quad_shader->set_uniform("u_model", debug_quad_transform.matrix());
    Shader::unuse_all();

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
    gldc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    _scene_shader->use();
    if (_rotate_light)
    {
        constexpr float light_orbit_radius = 50.0f;
        float light_y_tilt = glm::tan(glm::radians(-30.0f));
        float light_angle =
            std::fmod(glm::radians(Time::current_time() * 20.0f), glm::two_pi<float>());
        glm::vec3 new_direction = glm::vec3{light_orbit_radius * glm::cos(light_angle),
                                            light_orbit_radius * light_y_tilt,
                                            light_orbit_radius * glm::sin(light_angle)};
        _directional_light->set_direction(new_direction);
    }
    else
    {
        _directional_light->set_direction({45.0f, -30.0f, 20.0f});
    }

    // render depth of scene to texture from light's perspective
    constexpr float near_plane = 1.0f;
    constexpr float far_plane = 150.0f;
    glm::mat4 light_projection =
        glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, near_plane, far_plane);
    glm::vec3 light_position = _directional_light->position_from_direction(-50.0f);
    glm::mat4 light_view = glm::lookAt(light_position, glm::vec3(0.0f), {0.0f, 1.0f, 0.0f});
    glm::mat4 light_space_matrix = light_projection * light_view;

    // render scene from light's point of view
    gldc(glCullFace(GL_FRONT));
    _simple_depth_shader->use();
    _simple_depth_shader->set_uniform("u_light_space_matrix", light_space_matrix);

    gldc(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
    gldc(glBindFramebuffer(GL_FRAMEBUFFER, _depth_map_fbo));
    gldc(glClear(GL_DEPTH_BUFFER_BIT));
    render_scene(*_simple_depth_shader, false);
    gldc(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    gldc(glCullFace(GL_BACK));

    // reset viewport
    gldc(glViewport(0, 0, _window.buffer_width(), _window.buffer_height()));
    gldc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // render scene as normal using depth/shadow map
    _scene_shader->use();
    _scene_shader->set_uniform(_u_view_matrix, _camera->view());
    _scene_shader->set_uniform(_u_view_position, _camera->position());
    _scene_shader->set_uniform("u_light_space_matrix", light_space_matrix);
    _scene_shader->set_uniform("u_light_position", light_position);
    gldc(glActiveTexture(GL_TEXTURE4));
    gldc(glBindTexture(GL_TEXTURE_2D, _depth_map_tex));
    render_scene(*_scene_shader, true);

    // render debug quad
    if (_render_debug_quad)
    {
        _debug_quad_shader->use();

        // for perspective; we're using ortho so no need
        // _debug_quad_shader->set_uniform("u_near_plane", near_plane);
        // _debug_quad_shader->set_uniform("u_far_plane", far_plane);

        gldc(glActiveTexture(GL_TEXTURE0));
        gldc(glBindTexture(GL_TEXTURE_2D, _depth_map_tex));
        _debug_quad->draw(*_debug_quad_shader);
    }
}

void ShadowMappingLab::on_ui_render(UI& ui)
{
    movement_help_ui(ui);

    ui.begin_window("Shadow mapping");
    ImGui::Checkbox("Rotate light", &_rotate_light);
    ImGui::Checkbox("Show shadow map (debug)", &_render_debug_quad);
    ui.end_window();
}

void ShadowMappingLab::render_scene(Shader& shader, float use_materials)
{
    Transform ground_transform{};
    ground_transform.scale(glm::vec3(200.0f, 1.0f, 200.0f));
    shader.set_uniform("u_model", ground_transform.matrix());
    if (use_materials)
    {
        shader.set_uniform("u_texture_scale", 0.02f);
        _mat_dirt->bind(shader);
    }
    _ground->draw();
    if (use_materials)
    {
        _mat_dirt->unbind(shader);
    }

    Transform cube_transform{};
    // cube_transform.rotation(Time::current_time() * glm::radians(50.0f), {0.5f, 1.0f, 0.0f});
    if (use_materials)
    {
        shader.set_uniform("u_texture_scale", 1.0f);
        _mat_box->bind(shader);
    }
    for (glm::vec3 position : _cube_positions)
    {
        cube_transform.position(position);
        shader.set_uniform("u_model", cube_transform.matrix());
        _cube->draw();
    }
    if (use_materials)
    {
        _mat_box->unbind(shader);
    }
}

} // namespace eo

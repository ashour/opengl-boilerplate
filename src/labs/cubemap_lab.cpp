#include "config.h"
#include "cubemap_lab.h"
#include "lib/random.h"
#include "objects/primitive.h"
#include "registry/lab_registry.h"
#include "rendering/transform.h"
#include "system/time.h"
#include <stb_image.h>

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab(
        "Cubemaps", [](eo::Window& window) -> eo::Lab* { return new eo::CubemapLab(window); });
    return true;
}();
} // namespace

namespace eo
{

CubemapLab::CubemapLab(const Window& window) : Lab(window)
{
    gldc(glDisable(GL_CULL_FACE));
    _window.set_clear_color(SCENE_CLEAR_COLOR);

    _skybox_reflection_shader = std::make_shared<Shader>(
        "resources/shaders/skybox_reflection.vert", "resources/shaders/skybox_reflection.frag");
    _skybox_reflection_shader->build();

    _skybox_refraction_shader = std::make_shared<Shader>(
        "resources/shaders/skybox_refraction.vert", "resources/shaders/skybox_refraction.frag");
    _skybox_refraction_shader->build();

    _skybox_shader =
        std::make_shared<Shader>("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");
    _skybox_shader->build();

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _skybox_reflection_shader->use();
    _skybox_reflection_shader->set_uniform("u_projection", _camera->projection());

    _skybox_refraction_shader->use();
    _skybox_refraction_shader->set_uniform("u_projection", _camera->projection());

    _skybox_shader->use();
    _skybox_shader->set_uniform("u_projection", _camera->projection());

    Shader::unuse_all();

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

    _backpack = std::make_unique<Model>("resources/models/backpack/backpack.obj");

    std::vector<const std::string> cubemap_texture_face_filepaths{
        "resources/textures/skybox/right.jpg",
        "resources/textures/skybox/left.jpg",
        "resources/textures/skybox/top.jpg",
        "resources/textures/skybox/bottom.jpg",
        "resources/textures/skybox/front.jpg",
        "resources/textures/skybox/back.jpg",
    };
    _skybox_texture = load_cubemap(cubemap_texture_face_filepaths);

    _skybox = std::make_unique<Mesh>(Primitive::cube(), _mat_box);

    register_mouse_look(*_camera);
}

CubemapLab::~CubemapLab()
{
    gldc(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    gldc(glDeleteTextures(1, &_skybox_texture));
    gldc(glDepthFunc(GL_LESS));
    gldc(glEnable(GL_CULL_FACE));
}

void CubemapLab::on_update()
{
    toggle_movement();
    strafe_and_fly(*_camera);
}

void CubemapLab::on_render()
{
    gldc(glActiveTexture(GL_TEXTURE0));
    gldc(glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox_texture));

    gldc(glDepthFunc(GL_LESS));
    _skybox_refraction_shader->use();
    _skybox_refraction_shader->set_uniform("u_view", _camera->view());
    _skybox_refraction_shader->set_uniform("u_camera_position", _camera->position());
    _skybox_refraction_shader->set_uniform("u_skybox", 0);
    _skybox_refraction_shader->set_uniform("u_refractive_index", _refractive_index);

    Transform backpack_transform{};
    backpack_transform.position({0.0f, 3.0f, 4.0f});
    backpack_transform.scale(glm::vec3{2.0f});
    _skybox_refraction_shader->set_uniform("u_model", backpack_transform.matrix());
    _backpack->draw(*_skybox_refraction_shader, false);

    _skybox_reflection_shader->use();
    _skybox_reflection_shader->set_uniform("u_view", _camera->view());
    _skybox_reflection_shader->set_uniform("u_camera_position", _camera->position());
    _skybox_reflection_shader->set_uniform("u_skybox", 0);

    Transform cube_transform{};
    cube_transform.rotation(Time::current_time() * glm::radians(50.0f), {0.5f, 1.0f, 0.0f});

    for (glm::vec3 position : _cube_positions)
    {
        cube_transform.position(position);
        _skybox_reflection_shader->set_uniform("u_model", cube_transform.matrix());
        _cube->draw(*_skybox_reflection_shader, false);
    }

    gldc(glDepthFunc(GL_LEQUAL));
    _skybox_shader->use();
    _skybox_shader->set_uniform("u_skybox", 0);
    _skybox_shader->set_uniform("u_view", glm::mat4(glm::mat3(_camera->view())));
    _skybox->draw(*_skybox_shader, false);
}

void CubemapLab::on_ui_render(UI& ui)
{
    movement_help_ui(ui);

    ui.begin_window("Refractive index");
    ui.push_item_full_width();
    ui.drag_float("##refactive_index", &_refractive_index);
    ui.pop_item_full_width();
    ui.end_window();
}

unsigned int CubemapLab::load_cubemap(std::vector<const std::string>& face_texture_filepaths)
{
    unsigned int texture_id;

    gldc(glGenTextures(1, &texture_id));
    gldc(glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id));

    int width;
    int height;
    int channel_count;
    for (unsigned int i = 0; i < face_texture_filepaths.size(); i += 1)
    {
        unsigned char* data =
            stbi_load(face_texture_filepaths[i].c_str(), &width, &height, &channel_count, 0);
        if (data)
        {
            gldc(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                              0,
                              GL_RGB,
                              width,
                              height,
                              0,
                              GL_RGB,
                              GL_UNSIGNED_BYTE,
                              data));
        }
        else
        {
            EO_LOG_ERROR("Cubemap texture failed to load: {}", face_texture_filepaths[i]);
        }
        stbi_image_free(data);
    }

    gldc(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    gldc(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    gldc(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    gldc(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    gldc(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    return texture_id;
}
} // namespace eo

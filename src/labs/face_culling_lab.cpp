#include "config.h"
#include "face_culling_lab.h"
#include "objects/primitive.h"
#include "registry/lab_registry.h"
#include "rendering/transform.h"

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab("Face culling",
                                  [](eo::Window& window) -> eo::Lab*
                                  { return new eo::FaceCullingLab(window); });
    return true;
}();
} // namespace

namespace eo
{

FaceCullingLab::FaceCullingLab(const Window& window) : Lab(window)
{
    _window.set_clear_color(SCENE_CLEAR_COLOR);

    _shader = std::make_shared<Shader>("resources/shaders/unlit_texture.vert",
                                       "resources/shaders/unlit_texture.frag");
    _shader->build();

    _shader->use();
    _u_view_matrix = _shader->uniform_location_for("u_view");

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _shader->set_uniform("u_projection", _camera->projection());

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

    register_mouse_look_on_hold_rmb(*_camera);
}

FaceCullingLab::~FaceCullingLab() {}

void FaceCullingLab::on_update() { wasd_move_on_hold_rmb(*_camera); }

void FaceCullingLab::on_render()
{
    switch (_cull)
    {
    case Cull::none:
        gldc(glDisable(GL_CULL_FACE));
        break;
    case Cull::front:
        gldc(glEnable(GL_CULL_FACE));
        gldc(glCullFace(GL_FRONT));
        break;
    case Cull::back:
        gldc(glEnable(GL_CULL_FACE));
        gldc(glCullFace(GL_BACK));
        break;
    case Cull::front_and_back:
        gldc(glEnable(GL_CULL_FACE));
        gldc(glCullFace(GL_FRONT_AND_BACK));
        break;
    }

    _shader->use();
    _shader->set_uniform(_u_view_matrix, _camera->view());

    Transform ground_transform{};
    ground_transform.scale(glm::vec3(200.0f, 1.0f, 200.0f));
    _shader->set_uniform("u_model", ground_transform.matrix());
    _shader->set_uniform("u_texture_scale", 0.02f);
    _mat_dirt->bind(*_shader);
    _ground->draw();
    _mat_dirt->unbind(*_shader);

    Transform cube_transform{};
    cube_transform.scale(glm::vec3(10.0f));
    cube_transform.position({0.0f, 5.1f, 0.0f});
    _shader->set_uniform("u_texture_scale", 1.0f);
    _mat_box->bind(*_shader);
    _shader->set_uniform("u_model", cube_transform.matrix());
    _cube->draw();
    _mat_box->unbind(*_shader);
}

void FaceCullingLab::on_ui_render(UI& ui)
{
    ui.begin_window("Face culling");
    culling_radio_button(ui, "Disabled", Cull::none);
    culling_radio_button(ui, "Front", Cull::front);
    culling_radio_button(ui, "Back", Cull::back);
    culling_radio_button(ui, "Front and back", Cull::front_and_back);
    ui.end_window();
}

void FaceCullingLab::culling_radio_button(UI& ui, const std::string& label, Cull type)
{
    if (ui.radio_button(label, _cull == type))
    {
        _cull = type;
    }
}

} // namespace eo

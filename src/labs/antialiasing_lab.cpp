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

    _shader = std::make_shared<Shader>("resources/shaders/offscreen_msaa.vert",
                                       "resources/shaders/offscreen_msaa.frag");
    _shader->build();

    _shader->use();
    _u_view_matrix = _shader->uniform_location_for("u_view");

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _shader->set_uniform("u_projection", _camera->projection());

    Shader::unuse_all();

    _cube = std::make_unique<Mesh>(Primitive::cube());

    register_mouse_look(*_camera);
}

AntialiasingLab::~AntialiasingLab() {}

void AntialiasingLab::on_update()
{
    toggle_movement();
    strafe_and_fly(*_camera);
}

void AntialiasingLab::on_render()
{
    _shader->use();
    _shader->set_uniform(_u_view_matrix, _camera->view());

    Transform cube_transform{};
    cube_transform.scale(glm::vec3(10.0f));
    cube_transform.position({0.0f, 5.1f, 0.0f});
    cube_transform.rotation(glm::radians(20.0f), glm::vec3{0.6f, 0.2f, 0.2f});
    _shader->set_uniform("u_model", cube_transform.matrix());
    _cube->draw();
}

void AntialiasingLab::on_ui_render(UI& ui)
{
    movement_help_ui(ui);
    ui.begin_window("Antialiasing");
    ui.text("Make sure to set MULTISAMPLE_COUNT to 4 in config.h and re-run the app.");
    ui.end_window();
}

} // namespace eo

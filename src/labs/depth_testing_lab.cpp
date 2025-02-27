#include "config.h"
#include "depth_testing_lab.h"
#include "lib/random.h"
#include "objects/primitive.h"
#include "registry/lab_registry.h"
#include "rendering/transform.h"
#include "system/time.h"

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab("Depth testing",
                                  [](eo::Window& window) -> eo::Lab*
                                  { return new eo::DepthTestingLab(window); });
    return true;
}();
} // namespace

namespace eo
{

DepthTestingLab::DepthTestingLab(const Window& window) : Lab(window)
{
    gldc(glDepthFunc(GL_LESS));
    _window.set_clear_color(SCENE_CLEAR_COLOR);

    _shader = std::make_shared<Shader>("resources/shaders/depth_test.vert",
                                       "resources/shaders/depth_test.frag");
    _shader->build();

    _shader->use();
    _u_view_matrix = _shader->uniform_location_for("u_view");

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _shader->set_uniform("u_projection", _camera->projection());

    Shader::unuse_all();

    _ground = std::make_unique<Mesh>(Primitive::quad(), nullptr);

    _cube = std::make_unique<Mesh>(Primitive::cube(), nullptr);

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

void DepthTestingLab::on_update()
{
    toggle_movement();
    strafe_and_fly(*_camera);
}

void DepthTestingLab::on_render()
{
    _shader->use();
    _shader->set_uniform(_u_view_matrix, _camera->view());

    Transform cube_transform{};
    cube_transform.rotation(Time::current_time() * glm::radians(50.0f), {0.5f, 1.0f, 0.0f});
    for (glm::vec3 position : _cube_positions)
    {
        cube_transform.position(position);
        _shader->set_uniform("u_model", cube_transform.matrix());
        _cube->draw();
    }

    Transform ground_transform{};
    ground_transform.scale(glm::vec3(200.0f, 1.0f, 200.0f));
    _shader->set_uniform("u_model", ground_transform.matrix());
    _ground->draw();
}

void DepthTestingLab::on_ui_render(UI& ui) { movement_help_ui(ui); }

} // namespace eo

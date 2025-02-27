#include "config.h"
#include "objects/primitive.h"
#include "registry/lab_registry.h"
#include "rendering/transform.h"
#include "system/time.h"
#include "uniform_buffers_lab.h"

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab("Uniform Buffers",
                                  [](eo::Window& window) -> eo::Lab*
                                  { return new eo::UniformBuffersLab(window); });
    return true;
}();
} // namespace

namespace eo
{

UniformBuffersLab::UniformBuffersLab(const Window& window) : Lab(window)
{
    _window.enable_vsync(false);
    _window.set_clear_color(SCENE_CLEAR_COLOR);

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())));

    _red_shader = std::make_shared<Shader>("resources/shaders/uniform_buffer_red.vert",
                                           "resources/shaders/uniform_buffer_red.frag");
    _red_shader->build();
    _red_shader->uniform_block_binding("Matrices", 0);

    _blue_shader = std::make_shared<Shader>("resources/shaders/uniform_buffer_blue.vert",
                                            "resources/shaders/uniform_buffer_blue.frag");
    _blue_shader->build();
    _blue_shader->uniform_block_binding("Matrices", 0);

    _green_shader = std::make_shared<Shader>("resources/shaders/uniform_buffer_green.vert",
                                             "resources/shaders/uniform_buffer_green.frag");
    _green_shader->build();
    _green_shader->uniform_block_binding("Matrices", 0);

    _yellow_shader = std::make_shared<Shader>("resources/shaders/uniform_buffer_yellow.vert",
                                              "resources/shaders/uniform_buffer_yellow.frag");
    _yellow_shader->build();
    _yellow_shader->uniform_block_binding("Matrices", 0);

    _matrices_uniform_buffer = std::make_unique<UniformBuffer>(2 * sizeof(glm::mat4), nullptr);
    _matrices_uniform_buffer->bind_buffer_range(0, 0, 2 * sizeof(glm::mat4));
    _matrices_uniform_buffer->buffer_sub_data(
        0, sizeof(glm::mat4), glm::value_ptr(_camera->projection()));

    _cube = std::make_unique<Mesh>(Primitive::cube());

    register_mouse_look(*_camera);
}

UniformBuffersLab::~UniformBuffersLab() { _window.enable_vsync(true); }

void UniformBuffersLab::on_update()
{
    toggle_movement();
    strafe_and_fly(*_camera);
}

void UniformBuffersLab::on_render()
{

    _matrices_uniform_buffer->buffer_sub_data(
        sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(_camera->view()));

    Transform cube_transform{};
    cube_transform.rotation(Time::current_time() * glm::radians(50.0f), {0.5f, 1.0f, 0.0f});

    cube_transform.position(glm::vec3{-2.5f, 7.0f, 20.0f});
    _red_shader->use();
    _red_shader->set_uniform("u_model", cube_transform.matrix());
    _cube->draw();

    _blue_shader->use();
    cube_transform.position(glm::vec3{2.5f, 7.0f, 20.0f});
    _blue_shader->set_uniform("u_model", cube_transform.matrix());
    _cube->draw();

    _green_shader->use();
    cube_transform.position(glm::vec3{2.5f, 3.5f, 20.0f});
    _green_shader->set_uniform("u_model", cube_transform.matrix());
    _cube->draw();

    _yellow_shader->use();
    cube_transform.position(glm::vec3{-2.5f, 3.5f, 20.0f});
    _yellow_shader->set_uniform("u_model", cube_transform.matrix());
    _cube->draw();
}

void UniformBuffersLab::on_ui_render(UI& ui) { movement_help_ui(ui); }

} // namespace eo

#include "il_asteroids.h"
#include "rendering/transform.h"
#include "system/time.h"

namespace eo
{

IL_Asteroids::IL_Asteroids(const Window& window) : Lab(window)
{
    _window.set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())),
        glm::vec3{0.0f, 5.0f, 100.0f});

    _shader = std::make_unique<Shader>("resources/shaders/unlit_texture.vert",
                                       "resources/shaders/unlit_texture.frag");
    _shader->build();
    _shader->use();

    _shader->set_uniform("u_projection", _camera->projection());

    _planet = std::make_unique<Model>("resources/models/planet/planet.obj");
    _rock = std::make_unique<Model>("resources/models/rock/rock.obj");

    srand(Time::current_time());
    float radius = 50.0f;
    float offset = 2.5f;
    for (unsigned int i = 0; i < ROCK_COUNT; i += 1)
    {
        glm::mat4 model = glm::mat4{1.0f};
        float angle = (float)i / (float)ROCK_COUNT * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3{x, y, z});

        float scale = (rand() % 20) / 100.0f + 0.05f;
        model = glm::scale(model, glm::vec3{scale});

        float rotation_angle = (rand() % 360);
        model = glm::rotate(model, rotation_angle, glm::vec3{0.4f, 0.6f, 0.8f});

        _model_matrices[i] = model;
    }

    register_mouse_look(*_camera);
}

void IL_Asteroids::on_update()
{
    toggle_movement();
    strafe_and_fly(*_camera);
}

void IL_Asteroids::on_render()
{
    _shader->use();
    _shader->set_uniform("u_view", _camera->view());

    Transform planet_transform{};
    planet_transform.position(glm::vec3(10.0f, -3.0f, 30.0f));
    planet_transform.scale(glm::vec3{4.0f});
    _shader->set_uniform("u_model", planet_transform.matrix());
    _shader->set_uniform("u_texture_scale", 1.0f);
    _planet->draw(*_shader);
    _planet->unbind_materials(*_shader);

    for (unsigned int i = 0; i < ROCK_COUNT; i += 1)
    {
        _shader->set_uniform("u_model", _model_matrices[i]);
        _shader->set_uniform("u_texture_scale", 1.0f);
        _rock->draw(*_shader);
    }
    _rock->unbind_materials(*_shader);
}

void IL_Asteroids::on_ui_render(UI& ui) { movement_help_ui(ui); }

} // namespace eo

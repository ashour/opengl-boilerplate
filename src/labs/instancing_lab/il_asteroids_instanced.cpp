#include "il_asteroids_instanced.h"
#include "rendering/transform.h"
#include "system/time.h"

namespace eo
{

IL_AsteroidsInstanced::IL_AsteroidsInstanced(const Window& window) : Lab(window)
{
    _window.set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

    _camera = std::make_unique<Camera>(
        static_cast<float>(_window.buffer_width() / static_cast<float>(_window.buffer_height())),
        glm::vec3{0.0f, 5.0f, 100.0f});

    _planet_shader = std::make_unique<Shader>("resources/shaders/unlit_texture.vert",
                                              "resources/shaders/unlit_texture.frag");
    _planet_shader->build();
    _planet_shader->use();

    _planet_shader->set_uniform("u_projection", _camera->projection());

    _rock_shader = std::make_unique<Shader>("resources/shaders/unlit_texture_instanced.vert",
                                            "resources/shaders/unlit_texture_instanced.frag");
    _rock_shader->build();
    _rock_shader->use();

    _rock_shader->set_uniform("u_projection", _camera->projection());

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

    gldc(glGenBuffers(1, &_rock_instance_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, _rock_instance_vbo));
    gldc(glBufferData(
        GL_ARRAY_BUFFER, ROCK_COUNT * sizeof(glm::mat4), _model_matrices, GL_STATIC_DRAW));

    auto rock_meshes = _rock->meshes();
    for (unsigned int i = 0; i < rock_meshes.size(); i += 1)
    {
        gldc(glBindVertexArray(rock_meshes[i]->vao()));
        std::size_t vec4_size = sizeof(glm::vec4);
        gldc(glEnableVertexAttribArray(3));
        gldc(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)0));
        gldc(glEnableVertexAttribArray(4));
        gldc(
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(1 * vec4_size)));
        gldc(glEnableVertexAttribArray(5));
        gldc(
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(2 * vec4_size)));
        gldc(glEnableVertexAttribArray(6));
        gldc(
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(3 * vec4_size)));

        gldc(glVertexAttribDivisor(3, 1));
        gldc(glVertexAttribDivisor(4, 1));
        gldc(glVertexAttribDivisor(5, 1));
        gldc(glVertexAttribDivisor(6, 1));

        gldc(glBindVertexArray(0));
    }

    register_mouse_look(*_camera);
}

IL_AsteroidsInstanced::~IL_AsteroidsInstanced()
{
    gldc(glDeleteBuffers(1, &_rock_instance_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void IL_AsteroidsInstanced::on_update()
{
    toggle_movement();
    strafe_and_fly(*_camera);
}

void IL_AsteroidsInstanced::on_render()
{
    _planet_shader->use();
    _planet_shader->set_uniform("u_view", _camera->view());

    Transform planet_transform{};
    planet_transform.position(glm::vec3(10.0f, 3.0f, 30.0f));
    planet_transform.scale(glm::vec3{4.0f});
    _planet_shader->set_uniform("u_model", planet_transform.matrix());
    _planet_shader->set_uniform("u_texture_scale", 1.0f);
    _planet->draw(*_planet_shader);
    _planet->unbind_materials(*_planet_shader);

    _rock_shader->use();
    _rock_shader->set_uniform("u_view", _camera->view());

    auto rock_meshes = _rock->meshes();
    for (unsigned int i = 0; i < rock_meshes.size(); i += 1)
    {
        rock_meshes[i]->draw_instanced(*_rock_shader, ROCK_COUNT);
    }
    _rock->unbind_materials(*_rock_shader);
}

void IL_AsteroidsInstanced::on_ui_render(UI& ui) { movement_help_ui(ui); }

} // namespace eo

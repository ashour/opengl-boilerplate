#include "config.h"
#include "il_instanced_quads.h"
#include "system/metrics.h"

namespace eo
{

IL_InstancedQuads::IL_InstancedQuads(const Window& window) : Lab(window)
{
    _window.set_clear_color(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

    _shader = std::make_unique<Shader>("resources/shaders/instanced_quads.vert",
                                       "resources/shaders/instanced_quads.frag");
    _shader->build();
    _shader->use();

    // clang-format off
    float quad_vertices[] = {
        // positions     // colors
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        0.05f,  0.05f,  0.0f, 1.0f, 1.0f,
    };
    // clang-format on

    gldc(glGenVertexArrays(1, &_quad_vao));
    gldc(glBindVertexArray(_quad_vao));
    gldc(glGenBuffers(1, &_quad_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, _quad_vbo));
    gldc(glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW));
    gldc(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    gldc(glEnableVertexAttribArray(0));
    gldc(glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));
    gldc(glEnableVertexAttribArray(1));

    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index] = translation;
            index += 1;
        }
    }

    for (unsigned int i = 0; i < 100; i += 1)
    {
        _shader->set_uniform("offsets[" + std::to_string(i) + "]", translations[i]);
    }
}

IL_InstancedQuads::~IL_InstancedQuads()
{
    gldc(glDeleteVertexArrays(1, &_quad_vao));
    gldc(glBindVertexArray(0));
    gldc(glDeleteBuffers(1, &_quad_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void IL_InstancedQuads::on_render()
{
    gldc(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));
    if (SHOW_UI_METRICS)
    {
        Metrics::add_to_drawn_vertex_count(600);
    }
}

} // namespace eo

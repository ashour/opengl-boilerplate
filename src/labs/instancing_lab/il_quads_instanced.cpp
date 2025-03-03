#include "config.h"
#include "il_quads_instanced.h"
#include "system/metrics.h"

namespace eo
{

IL_QuadsInstanced::IL_QuadsInstanced(const Window& window) : Lab(window)
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

    _vertex_array = std::make_unique<VertexArray>(sizeof(quad_vertices), quad_vertices);
    _vertex_array->attribute_pointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    _vertex_array->attribute_pointer(
        1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

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

void IL_QuadsInstanced::on_render()
{
    gldc(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));
    if (SHOW_UI_METRICS)
    {
        Metrics::add_to_drawn_vertex_count(600);
    }
}

} // namespace eo

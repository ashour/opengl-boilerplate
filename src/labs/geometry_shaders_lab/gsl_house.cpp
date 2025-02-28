#include "config.h"
#include "gsl_house.h"
#include "system/metrics.h"

namespace eo
{

Gsl_House::Gsl_House(const Window& window) : Lab(window)
{
    _window.set_clear_color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    _points_shader = std::make_unique<Shader>("resources/shaders/house.vert",
                                              "resources/shaders/house.frag",
                                              "resources/shaders/house.geom");
    _points_shader->build();
    _points_shader->use();

    // clang-format off
    float points[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f
    };
    // clang-format on
    gldc(glGenVertexArrays(1, &_points_vao));
    gldc(glBindVertexArray(_points_vao));
    gldc(glGenBuffers(1, &_points_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, _points_vbo));
    gldc(glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW));
    gldc(glEnableVertexAttribArray(0));
    gldc(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    gldc(glEnableVertexAttribArray(1));
    gldc(glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));
}

Gsl_House::~Gsl_House()
{
    gldc(glDeleteVertexArrays(1, &_points_vao));
    gldc(glBindVertexArray(0));
    gldc(glDeleteBuffers(1, &_points_vbo));
}

void Gsl_House::on_render()
{
    gldc(glDrawArrays(GL_POINTS, 0, 4));
    if (SHOW_UI_METRICS)
    {
        Metrics::add_to_drawn_vertex_count(4);
    }
}

} // namespace eo

#include "config.h"
#include "gsl_points.h"
#include "system/metrics.h"

namespace eo
{

Gsl_Points::Gsl_Points(const Window& window) : Lab(window)
{
    _window.set_clear_color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    _points_shader = std::make_unique<Shader>("resources/shaders/points.vert",
                                              "resources/shaders/points.frag",
                                              "resources/shaders/points.geom");
    _points_shader->build();
    _points_shader->use();

    // clang-format off
    float points[] = {
        -0.5f,  0.5f,
         0.5f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f,
    };
    // clang-format on
    gldc(glGenVertexArrays(1, &_points_vao));
    gldc(glBindVertexArray(_points_vao));
    unsigned int points_vbo;
    gldc(glGenBuffers(1, &points_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, points_vbo));
    gldc(glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW));
    gldc(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
    gldc(glEnableVertexAttribArray(0));
}

Gsl_Points::~Gsl_Points()
{
    gldc(glDeleteVertexArrays(1, &_points_vao));
    gldc(glBindVertexArray(0));
}

void Gsl_Points::on_render()
{
    gldc(glDrawArrays(GL_POINTS, 0, 4));
    if (SHOW_UI_METRICS)
    {
        Metrics::add_to_drawn_vertex_count(4);
    }
}

} // namespace eo

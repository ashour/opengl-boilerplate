#include "config.h"
#include "mesh.h"
#include "rendering/rendering.h"
#include "system/metrics.h"
#include <vector>

namespace eo
{
Mesh::Mesh(const Object& object) : Mesh(object.vertices, object.indices, nullptr) {}
Mesh::Mesh(const Object& object, std::shared_ptr<Material> material)
    : Mesh(object.vertices, object.indices, material)
{
}

Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<unsigned int> indices,
           std::shared_ptr<Material> material)
    : _vertices{vertices}, _indices{indices}, _material(material)
{
    init_vertex_array();
    init_buffers();
    init_vertex_attribs();
}

Mesh::~Mesh()
{
    gldc(glDeleteVertexArrays(1, &_vao));
    gldc(glDeleteBuffers(1, &_vbo));
    gldc(glDeleteBuffers(1, &_ebo));
}

void Mesh::draw()
{
    gldc(glBindVertexArray(_vao));
    gldc(glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0));

    if (SHOW_UI_METRICS)
    {
        Metrics::add_to_drawn_vertex_count(_indices.size());
    }
}

void Mesh::draw(Shader& shader, bool use_material)
{
    if (use_material && _material)
    {
        _material->bind(shader);
    }

    gldc(glBindVertexArray(_vao));
    gldc(glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0));

    if (SHOW_UI_METRICS)
    {
        Metrics::add_to_drawn_vertex_count(_indices.size());
    }
}

void Mesh::init_vertex_array()
{
    gldc(glGenVertexArrays(1, &_vao));
    gldc(glBindVertexArray(_vao));
}

void Mesh::init_buffers()
{
    gldc(glGenBuffers(1, &_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
    gldc(glBufferData(
        GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW));

    gldc(glGenBuffers(1, &_ebo));
    gldc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
    gldc(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      _indices.size() * sizeof(unsigned int),
                      &_indices[0],
                      GL_STATIC_DRAW));
}

void Mesh::init_vertex_attribs() const
{
    gldc(glEnableVertexAttribArray(0));
    gldc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

    gldc(glEnableVertexAttribArray(1));
    gldc(glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal))));

    gldc(glEnableVertexAttribArray(2));
    gldc(glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv))));
}
} // namespace eo

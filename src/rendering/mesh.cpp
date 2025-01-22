#include "lib/opengl_debug.h"
#include "mesh.h"
#include <glad/glad.h>
#include <vector>

namespace eo
{
void Mesh::init(const float* vertices,
                const size_t vertex_count,
                const unsigned int* indices,
                const size_t index_count,
                const std::vector<VertexAttribute>& attributes)
{
    gldc(glGenVertexArrays(1, &_vao));
    gldc(glBindVertexArray(_vao));

    gldc(glGenBuffers(1, &_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
    gldc(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count, vertices, GL_STATIC_DRAW));

    gldc(glGenBuffers(1, &_ebo));
    gldc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
    gldc(glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_count, indices, GL_STATIC_DRAW));

    for (auto attribute : attributes)
    {
        attribute.init();
        attribute.enable();
    }
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
    gldc(glDrawElements(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, 0));
}
} // namespace eo

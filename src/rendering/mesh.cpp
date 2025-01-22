#include "lib/opengl_debug.h"
#include "mesh.h"
#include "vertex_attribute.h"
#include <glad/glad.h>
#include <vector>

namespace eo
{
Mesh::Mesh(const Object& object)
{
    _index_count = object.indices.size();

    gldc(glGenVertexArrays(1, &_vao));
    gldc(glBindVertexArray(_vao));

    gldc(glGenBuffers(1, &_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
    gldc(glBufferData(GL_ARRAY_BUFFER,
                      sizeof(float) * object.vertices.size(),
                      object.vertices.data(),
                      GL_STATIC_DRAW));

    gldc(glGenBuffers(1, &_ebo));
    gldc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
    gldc(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      sizeof(unsigned int) * object.indices.size(),
                      object.indices.data(),
                      GL_STATIC_DRAW));

    for (auto object_attribute : object.attributes)
    {
        VertexAttr{object_attribute}.enable();
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

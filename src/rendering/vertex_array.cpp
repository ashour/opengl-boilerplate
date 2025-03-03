#include "vertex_array.h"

namespace eo
{

VertexArray::VertexArray(GLsizeiptr size, const void* data)
{
    gldc(glGenVertexArrays(1, &_vao));
    gldc(glBindVertexArray(_vao));
    gldc(glGenBuffers(1, &_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
    gldc(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexArray::~VertexArray()
{
    gldc(glDeleteVertexArrays(1, &_vao));
    gldc(glBindVertexArray(0));
    gldc(glDeleteBuffers(1, &_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexArray::attribute_pointer(GLuint index,
                                    GLint size,
                                    GLenum type,
                                    GLboolean normalized,
                                    GLsizei stride,
                                    const void* pointer)
{
    gldc(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
    gldc(glEnableVertexAttribArray(index));
}
} // namespace eo

#include "uniform_buffer.h"

namespace eo
{
UniformBuffer::UniformBuffer(GLsizeiptr size, const void* data)
{
    gldc(glGenBuffers(1, &_ubo));
    gldc(glBindBuffer(GL_UNIFORM_BUFFER, _ubo));
    gldc(glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW));
    gldc(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

UniformBuffer::~UniformBuffer()
{
    gldc(glDeleteBuffers(1, &_ubo));
    gldc(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::bind_buffer_range(unsigned int index, GLintptr offset, GLsizeiptr size)
{
    gldc(glBindBufferRange(GL_UNIFORM_BUFFER, index, _ubo, offset, size));
}

void UniformBuffer::buffer_sub_data(GLintptr offset, GLsizeiptr size, const void* data)
{
    gldc(glBindBuffer(GL_UNIFORM_BUFFER, _ubo));
    gldc(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
    gldc(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}
} // namespace eo

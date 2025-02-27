#pragma once

#include "rendering.h"

namespace eo
{
class UniformBuffer
{
  public:
    UniformBuffer(GLsizeiptr size, const void* data);
    ~UniformBuffer();

    void bind_buffer_range(unsigned int index, GLintptr offset, GLsizeiptr size);
    void buffer_sub_data(GLintptr offset, GLsizeiptr size, const void* data);

  private:
    unsigned int _ubo;
};
} // namespace eo

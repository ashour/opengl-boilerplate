#pragma once

#include <glad/glad.h>

namespace eo
{
enum class VertexAttributeType
{
    FLOAT = GL_FLOAT
};

struct VertexAttributeProps
{
    const unsigned int index;
    const unsigned int size;
    const VertexAttributeType type;
    const bool is_normalized;
    const unsigned int stride;
    const void* pointer;
};

class VertexAttribute
{
  public:
    VertexAttribute(const VertexAttributeProps& props);
    void init();
    void enable();

  private:
    const VertexAttributeProps& _props;
};
} // namespace eo

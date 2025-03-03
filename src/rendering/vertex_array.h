#include "rendering/rendering.h"

namespace eo
{
class VertexArray
{
  public:
    VertexArray(GLsizeiptr size, const void* data);
    ~VertexArray();

    void attribute_pointer(GLuint index,
                           GLint size,
                           GLenum type,
                           GLboolean normalized,
                           GLsizei stride,
                           const void* pointer);

  private:
    unsigned int _vao;
    unsigned int _vbo;
};
} // namespace eo

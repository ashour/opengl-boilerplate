#include "lib/opengl_debug.h"
#include "vertex_attribute.h"
#include <glad/glad.h>

namespace eo
{
VertexAttribute::VertexAttribute(const VertexAttributeProps& props) : _props{props} {}

void VertexAttribute::init()
{
    gldc(glVertexAttribPointer(_props.index,
                               _props.size,
                               static_cast<GLenum>(_props.type),
                               _props.is_normalized,
                               _props.stride,
                               _props.pointer));
}

void VertexAttribute::enable() { gldc(glEnableVertexAttribArray(_props.index)); }
} // namespace eo

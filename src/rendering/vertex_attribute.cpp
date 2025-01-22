#include "lib/opengl_debug.h"
#include "objects/object_attribute.h"
#include "vertex_attribute.h"
#include <glad/glad.h>

namespace eo
{
VertexAttr::VertexAttr(const ObjectAttr& object_attribute) : _object_attribute(object_attribute) {}

void VertexAttr::enable()
{
    gldc(glVertexAttribPointer(_object_attribute.index,
                               _object_attribute.size,
                               to_gl_enum(_object_attribute.type),
                               _object_attribute.is_normalized,
                               _object_attribute.stride,
                               _object_attribute.pointer));

    gldc(glEnableVertexAttribArray(_object_attribute.index));
}

GLenum VertexAttr::to_gl_enum(const ObjectAttrType& type)
{
    switch (type)
    {
    case ObjectAttrType::FLOAT:
        return GL_FLOAT;
    }
}
} // namespace eo

#pragma once

#include "objects/object_attribute.h"
#include <glad/glad.h>

namespace eo
{
class VertexAttr
{
  public:
    VertexAttr(const ObjectAttr& object_attribute);

    void enable();

  private:
    static GLenum to_gl_enum(const ObjectAttrType& type);

    const ObjectAttr& _object_attribute;
};
} // namespace eo

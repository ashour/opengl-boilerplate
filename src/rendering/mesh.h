#pragma once

#include "objects/object.h"

namespace eo
{

class Mesh
{
  public:
    explicit Mesh(const Object& object);
    ~Mesh();

    void draw();

  private:
    unsigned int _vao{};
    unsigned int _vbo{};
    unsigned int _ebo{};

    size_t _index_count{};
};
} // namespace eo

#pragma once

#include "objects/object.h"
#include "vertex_attribute.h"
#include <vector>

namespace eo
{

class Mesh
{
  public:
    template <size_t V, size_t I>
    explicit Mesh(const Object<V, I>& object, const std::vector<VertexAttribute>& attributes)
    {
        _index_count = object.indices.size();

        init(object.vertices.data(),
             object.vertices.size(),
             object.indices.data(),
             object.indices.size(),
             attributes);
    }

    ~Mesh();

    void draw();

  private:
    unsigned int _vao{};
    unsigned int _vbo{};
    unsigned int _ebo{};

    size_t _index_count{};

    void init(const float* vertices,
              const size_t vertex_count,
              const unsigned int* indices,
              const size_t index_count,
              const std::vector<VertexAttribute>& attributes);
};
} // namespace eo

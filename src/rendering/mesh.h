#pragma once

#include "objects/object.h"
#include "rendering/material.h"
#include "rendering/shader.h"
#include "rendering/vertex.h"
#include <memory>

namespace eo
{

class Mesh
{
  public:
    explicit Mesh(const Object& object, std::shared_ptr<Material> material);
    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::shared_ptr<Material> material);
    ~Mesh();

    void draw();
    void draw(Shader& shader);

    const std::vector<Vertex>& vertices() const { return _vertices; }
    const std::vector<unsigned int>& indices() const { return _indices; }
    const std::shared_ptr<Material> material() const { return _material; }

  private:
    unsigned int _vao{};
    unsigned int _vbo{};
    unsigned int _ebo{};

    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::shared_ptr<Material> _material;

    void init_vertex_array();
    void init_buffers();
    void init_vertex_attribs() const;
};
} // namespace eo

#pragma once

#include "objects/object.h"
#include "rendering/new_texture.h"
#include "rendering/shader.h"
#include "rendering/vertex.h"

namespace eo
{

class Mesh
{
  public:
    explicit Mesh(const Object& object);
    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<NewTexture> textures,
         float shininess);
    ~Mesh();

    void draw();
    void draw(Shader& shader);

    const std::vector<Vertex>& vertices() const { return _vertices; }
    const std::vector<unsigned int>& indices() const { return _indices; }
    const std::vector<NewTexture>& textures() const { return _textures; }

  private:
    unsigned int _vao{};
    unsigned int _vbo{};
    unsigned int _ebo{};

    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<NewTexture> _textures;

    float _shininess{};

    void init_vertex_array();
    void init_buffers();
    void init_vertex_attribs() const;
};
} // namespace eo

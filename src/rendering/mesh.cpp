#include "lib/opengl_debug.h"
#include "mesh.h"
#include <glad/glad.h>
#include <vector>

namespace eo
{
Mesh::Mesh(const Object& object)
    : Mesh(object.vertices, object.indices, std::vector<NewTexture>{}, 0.0f)
{
}

Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<unsigned int> indices,
           std::vector<NewTexture> textures,
           float shininess)
    : _vertices{vertices}, _indices{indices}, _textures{textures}, _shininess{shininess}
{
    init_vertex_array();
    init_buffers();
    init_vertex_attribs();
}

Mesh::~Mesh()
{
    gldc(glDeleteVertexArrays(1, &_vao));
    gldc(glDeleteBuffers(1, &_vbo));
    gldc(glDeleteBuffers(1, &_ebo));
}

void Mesh::draw()
{
    gldc(glBindVertexArray(_vao));
    gldc(glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0));
}

void Mesh::draw(Shader& shader)
{
    unsigned int diffuse_counter = 1;
    unsigned int specular_counter = 1;

    for (int i = 0; i < _textures.size(); i += 1)
    {
        gldc(glActiveTexture(GL_TEXTURE0 + i));

        std::string number;
        std::string name = _textures[i].type;

        // TODO enum instead of magic values here
        if (name == "diffuse")
        {
            number = std::to_string(diffuse_counter);
            diffuse_counter += 1;
        }
        else if (name == "specular")
        {
            number = std::to_string(specular_counter);
            specular_counter += 1;
        }

        // TODO constant template instead of magic value here
        shader.set_uniform(std::format("u_material.{}_{}", name, number), i);
        gldc(glBindTexture(GL_TEXTURE_2D, _textures[i].id));
    }
    gldc(glActiveTexture(GL_TEXTURE0));

    shader.set_uniform(std::format("u_material.shininess"), _shininess);

    gldc(glBindVertexArray(_vao));
    gldc(glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0));
}

void Mesh::init_vertex_array()
{
    gldc(glGenVertexArrays(1, &_vao));
    gldc(glBindVertexArray(_vao));
}

void Mesh::init_buffers()
{
    gldc(glGenBuffers(1, &_vbo));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
    gldc(glBufferData(
        GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW));

    gldc(glGenBuffers(1, &_ebo));
    gldc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
    gldc(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      _indices.size() * sizeof(unsigned int),
                      &_indices[0],
                      GL_STATIC_DRAW));
}

void Mesh::init_vertex_attribs() const
{
    gldc(glEnableVertexAttribArray(0));
    gldc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

    gldc(glEnableVertexAttribArray(1));
    gldc(glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal))));

    gldc(glEnableVertexAttribArray(2));
    gldc(glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv))));
}
} // namespace eo

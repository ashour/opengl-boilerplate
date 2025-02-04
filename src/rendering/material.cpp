#include "material.h"
#include "rendering/shader.h"
#include "rendering/texture.h"
#include <glm/glm.hpp>
#include <memory>

namespace eo
{
Material::Material(const std::vector<std::shared_ptr<Texture>>& textures, const float shininess)
    : _textures(textures), _shininess{shininess}
{
}

void Material::bind(Shader& shader)
{
    shader.set_uniform("u_material.diffuse_1", 0);
    shader.set_uniform("u_material.specular_1", 1);

    for (auto texture : _textures)
    {
        if (texture->type == "diffuse")
        {
            gldc(glActiveTexture(GL_TEXTURE0));
        }
        else if (texture->type == "specular")
        {
            gldc(glActiveTexture(GL_TEXTURE1));
        }
        gldc(glBindTexture(GL_TEXTURE_2D, texture->id));
    }

    shader.set_uniform("u_material.shininess", _shininess);
}
} // namespace eo

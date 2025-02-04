#include "material.h"
#include "rendering/shader.h"
#include "rendering/texture.h"
#include <glm/glm.hpp>
#include <memory>

namespace eo
{
std::unique_ptr<Texture> Material::_black_pixel = nullptr;

Material::Material(const std::vector<std::shared_ptr<Texture>>& textures, const float shininess)
    : _textures(textures), _shininess{shininess}
{
    if (!_black_pixel)
    {
        const unsigned char black[] = {0, 0, 0, 255};
        _black_pixel = std::make_unique<Texture>(black, 1, 1);
    }
}

void Material::bind(Shader& shader)
{
    for (auto texture : _textures)
    {
        switch (texture->type())
        {
        case Texture::Type::diffuse:
            shader.set_uniform("u_material.diffuse_1", 2);
            gldc(glActiveTexture(GL_TEXTURE2));
            gldc(glBindTexture(GL_TEXTURE_2D, texture->id()));
            break;
        case Texture::Type::specular:
            shader.set_uniform("u_material.specular_1", 3);
            gldc(glActiveTexture(GL_TEXTURE3));
            gldc(glBindTexture(GL_TEXTURE_2D, texture->id()));
            break;
        case Texture::Type::no_specular:
            shader.set_uniform("u_material.specular_1", 3);
            gldc(glActiveTexture(GL_TEXTURE3));
            gldc(glBindTexture(GL_TEXTURE_2D, _black_pixel->id()));
            break;
        default:
            EO_LOG_ERROR("Unsupported texture type!");
        }
    }

    shader.set_uniform("u_material.shininess", _shininess);
}

void Material::unbind(Shader& shader)
{
    for (auto texture : _textures)
    {
        switch (texture->type())
        {
        case Texture::Type::diffuse:
            gldc(glActiveTexture(GL_TEXTURE2));
            gldc(glBindTexture(GL_TEXTURE_2D, 0));
            break;
        case Texture::Type::specular:
            gldc(glActiveTexture(GL_TEXTURE3));
            gldc(glBindTexture(GL_TEXTURE_2D, 0));
            break;
        case Texture::Type::no_specular:
            gldc(glActiveTexture(GL_TEXTURE3));
            gldc(glBindTexture(GL_TEXTURE_2D, 0));
            break;
        default:
            EO_LOG_ERROR("Unsupported texture type!");
        }
    }

    shader.set_uniform("u_material.diffuse_1", 0);
    shader.set_uniform("u_material.specular_1", 0);

    shader.set_uniform("u_material.shininess", 0.0f);
}
} // namespace eo

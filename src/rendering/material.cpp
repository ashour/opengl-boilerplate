#include "material.h"
#include "rendering/texture.h"
#include <glm/glm.hpp>
#include <memory>

namespace eo
{
Material::Material(Shader& shader,
                   const std::string& diffuse_texture_file_path,
                   const Format& diffuse_texture_format,
                   const float shininess,
                   const std::string& specular_texture_file_path,
                   const Format& specular_texture_format)
    : _shader(shader),
      _shininess{shininess},
      _tex_diffuse{std::make_unique<Texture>(diffuse_texture_file_path, diffuse_texture_format)},
      _tex_specular{
          specular_texture_file_path.empty()
              ? std::make_unique<Texture>(black_pixel())
              : std::make_unique<Texture>(specular_texture_file_path, specular_texture_format)}
{
    _shader.set_uniform("u_material.diffuse", 0);
    _shader.set_uniform("u_material.specular", 1);
}

void Material::use() const
{
    _tex_diffuse->bind(TextureUnit::TEXUNIT0);
    _tex_specular->bind(TextureUnit::TEXUNIT1);

    _shader.set_uniform("u_material.shininess", _shininess);
}

Texture& Material::black_pixel()
{
    static std::unique_ptr<Texture> black_pixel = []()
    {
        unsigned char black[] = {0, 0, 0, 255};
        return std::make_unique<Texture>(black, 1, 1, Format::RGBA);
    }();

    return *black_pixel;
}
} // namespace eo

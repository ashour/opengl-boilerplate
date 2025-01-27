#include "material.h"
#include "rendering/texture.h"
#include <glm/glm.hpp>

namespace eo
{
Material::Material(const Shader& shader,
                   const Texture& diffuse_texture,
                   const Texture& specular_texture,
                   const float shininess)
    : _shader(shader),
      _diffuse_texture{diffuse_texture},
      _specular_texture{specular_texture},
      _shininess{shininess}
{
    _u_diffuse = _shader.uniform_location_for("u_material.diffuse");
    _u_specular = _shader.uniform_location_for("u_material.specular");
    _u_shininess = _shader.uniform_location_for("u_material.shininess");

    _shader.set_uniform_1i(_u_diffuse, 0);
    _shader.set_uniform_1i(_u_specular, 1);
}

void Material::use() const
{
    _diffuse_texture.bind(TextureUnit::TEXUNIT0);
    _specular_texture.bind(TextureUnit::TEXUNIT1);

    _shader.set_uniform_1f(_u_shininess, _shininess);
}
} // namespace eo

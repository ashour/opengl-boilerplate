#pragma once

#include "rendering/shader.h"
#include "rendering/texture.h"
#include <glm/glm.hpp>

namespace eo
{
class Material
{
  public:
    Material(const Shader& shader,
             const Texture& diffuse_texture,
             const Texture& specular_texture,
             const float shininess);

    void use() const;

  private:
    const Shader& _shader;
    const Texture& _diffuse_texture;
    const Texture& _specular_texture;
    const float _shininess{};

    unsigned int _u_diffuse;
    unsigned int _u_specular;
    unsigned int _u_shininess;
};
} // namespace eo

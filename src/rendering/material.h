#pragma once

#include "rendering/shader.h"
#include "rendering/texture.h"
#include <glm/glm.hpp>
#include <memory>

namespace eo
{
class Material
{
  public:
    Material(const Shader& shader,
             const std::string& diffuse_texture_file_path,
             const Format& diffuse_texture_format,
             const std::string& specular_texture_file_path,
             const Format& specular_texture_format,
             const float shininess);

    void use() const;

  private:
    const Shader& _shader;
    const std::unique_ptr<Texture> _diffuse_texture;
    const std::unique_ptr<Texture> _specular_texture;
    const float _shininess{};

    unsigned int _u_diffuse;
    unsigned int _u_specular;
    unsigned int _u_shininess;
};
} // namespace eo

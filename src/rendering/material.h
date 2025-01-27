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

    Material(const Shader& shader,
             const std::string& diffuse_texture_file_path,
             const Format& diffuse_texture_format,
             const float shininess);

    void use() const;

  private:
    static Texture& black_pixel();

    const Shader& _shader;
    const std::unique_ptr<Texture> _tex_diffuse;
    const std::unique_ptr<Texture> _tex_specular;
    const float _shininess{};

    unsigned int _u_diffuse;
    unsigned int _u_specular;
    unsigned int _u_shininess;

    static Texture& _s_tex_black_pixel;
};
} // namespace eo

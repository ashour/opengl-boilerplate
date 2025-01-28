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
    Material(Shader& shader,
             const std::string& diffuse_texture_file_path,
             const Format& diffuse_texture_format,
             const float shininess,
             const std::string& specular_texture_file_path = "",
             const Format& specular_texture_format = Format::RGBA);

    void use() const;

  private:
    void init_uniforms();

    static Texture& black_pixel();

    Shader& _shader;
    const std::unique_ptr<Texture> _tex_diffuse;
    const std::unique_ptr<Texture> _tex_specular;
    const float _shininess{};

    int _u_diffuse;
    int _u_specular;
    int _u_shininess;

    static Texture& _s_tex_black_pixel;
};
} // namespace eo

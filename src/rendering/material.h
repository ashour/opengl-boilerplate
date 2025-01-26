#pragma once

#include "rendering/shader.h"
#include <glm/glm.hpp>

namespace eo
{
class Material
{
  public:
    Material(const Shader& shader,
             const glm::vec3& diffuse_color,
             const glm::vec3& specular_color,
             const float shininess);

    void use() const;

  private:
    const Shader& _shader;
    const glm::vec3 _diffuse_color{};
    const glm::vec3 _specular_color{};
    const float _shininess{};

    unsigned int _u_diffuse;
    unsigned int _u_specular;
    unsigned int _u_shininess;
};
} // namespace eo

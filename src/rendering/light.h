#pragma once

#include "rendering/shader.h"
#include <glm/glm.hpp>

namespace eo
{
class Light
{
  public:
    Light(const Shader& shader,
          const glm::vec3& direction,
          const glm::vec3& ambient_color,
          const glm::vec3& diffuse_color,
          const glm::vec3& specular_color);

    void set_direction(const glm::vec3& new_direction);

  private:
    const Shader& _shader;

    unsigned int _u_direction{};
    unsigned int _u_ambient{};
    unsigned int _u_diffuse{};
    unsigned int _u_specular{};
};
} // namespace eo

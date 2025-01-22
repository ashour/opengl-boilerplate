#pragma once

#include "rendering/shader.h"
#include <glm/glm.hpp>

namespace eo
{
class Light
{
  public:
    Light(const Shader& shader,
          const glm::vec3& position,
          const glm::vec3& ambient_color,
          const glm::vec3& diffuse_color,
          const glm::vec3& specular_color);

    void set_position(const glm::vec3& new_position);

  private:
    const Shader& _shader;

    unsigned int _u_position{};
    unsigned int _u_ambient{};
    unsigned int _u_diffuse{};
    unsigned int _u_specular{};
};
} // namespace eo

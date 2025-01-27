#pragma once

#include "rendering/shader.h"
#include <glm/glm.hpp>
#include <string>

namespace eo
{
class Light
{
  protected:
    Light(const Shader& shader,
          const std::string& name,
          const glm::vec3& ambient_color,
          const glm::vec3& diffuse_color,
          const glm::vec3& specular_color);

    const Shader& _shader;
};
} // namespace eo

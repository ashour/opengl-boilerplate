#pragma once

#include "light.h"

namespace eo
{
class PointLight : public Light
{
  public:
    PointLight(Shader& shader,
               const std::string& name,
               const glm::vec3& ambient_color,
               const glm::vec3& diffuse_color,
               const glm::vec3& specular_color,
               const glm::vec3& position,
               const float constant,
               const float linear,
               const float quadratic);
};
} // namespace eo

#pragma once

#include "rendering/lights/light.h"
#include <glm/glm.hpp>
#include <string>

namespace eo
{

class DirectionalLight : public Light
{
  public:
    DirectionalLight(const Shader& shader,
                     const std::string& name,
                     const glm::vec3& direction,
                     const glm::vec3& ambient_color,
                     const glm::vec3& diffuse_color,
                     const glm::vec3& specular_color);

    void set_direction(const glm::vec3& new_direction);

  private:
    int _u_direction{};
};
} // namespace eo

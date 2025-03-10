#pragma once

#include "light.h"
#include <glm/glm.hpp>
#include <string>

namespace eo
{

class DirectionalLight : public Light
{
  public:
    DirectionalLight(Shader& shader,
                     const std::string& name,
                     const glm::vec3& direction,
                     const glm::vec3& ambient_color,
                     const glm::vec3& diffuse_color,
                     const glm::vec3& specular_color);

    void set_direction(const glm::vec3& new_direction);

    glm::vec3 position_from_direction(float distance);

  private:
    glm::vec3 _direction;
    int _u_direction{};
};
} // namespace eo

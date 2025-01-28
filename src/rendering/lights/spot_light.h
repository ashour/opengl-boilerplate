#pragma once

#include "light.h"

namespace eo
{
class SpotLight : public Light
{
  public:
    SpotLight(Shader& shader,
              const std::string& name,
              const glm::vec3& ambient_color,
              const glm::vec3& diffuse_color,
              const glm::vec3& specular_color,
              const glm::vec3& position,
              const glm::vec3& direction,
              const float inner_cutoff);

    void set_position(const glm::vec3& new_position) const;
    void set_direction(const glm::vec3& new_direction) const;

  private:
    int _u_position;
    int _u_direction;
};
} // namespace eo

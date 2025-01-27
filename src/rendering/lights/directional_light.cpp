#include "directional_light.h"
#include <format>
#include <string>

namespace eo
{
DirectionalLight::DirectionalLight(const Shader& shader,
                                   const std::string& name,
                                   const glm::vec3& direction,
                                   const glm::vec3& ambient_color,
                                   const glm::vec3& diffuse_color,
                                   const glm::vec3& specular_color)
    : Light(shader, name, ambient_color, diffuse_color, specular_color)
{
    _u_direction = shader.uniform_location_for(std::format("{}.direction", name));
    shader.set_uniform_vec3(_u_direction, direction);
}

void DirectionalLight::set_direction(const glm::vec3& new_direction)
{
    _shader.set_uniform_vec3(_u_direction, new_direction);
}
} // namespace eo

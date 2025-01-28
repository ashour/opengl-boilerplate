#include "spot_light.h"
#include <format>
#include <glm/glm.hpp>

namespace eo
{
SpotLight::SpotLight(Shader& shader,
                     const std::string& name,
                     const glm::vec3& ambient_color,
                     const glm::vec3& diffuse_color,
                     const glm::vec3& specular_color,
                     const glm::vec3& position,
                     const float constant,
                     const float linear,
                     const float quadratic,
                     const glm::vec3& direction,
                     const float inner_cutoff)
    : Light{shader, name, ambient_color, diffuse_color, specular_color}
{
    _u_position = shader.uniform_location_for(std::format("{}.position", name));
    _u_direction = shader.uniform_location_for(std::format("{}.direction", name));

    shader.set_uniform(_u_position, position);
    shader.set_uniform(_u_direction, direction);

    shader.set_uniform(std::format("{}.constant", name), constant);
    shader.set_uniform(std::format("{}.linear", name), linear);
    shader.set_uniform(std::format("{}.quadratic", name), quadratic);
    shader.set_uniform(std::format("{}.inner_cutoff", name), inner_cutoff);
}

void SpotLight::set_position(const glm::vec3& new_position) const
{
    _shader.set_uniform(_u_position, new_position);
}

void SpotLight::set_direction(const glm::vec3& new_direction) const
{
    _shader.set_uniform(_u_direction, new_direction);
}
} // namespace eo

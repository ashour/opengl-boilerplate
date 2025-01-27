#include "light.h"
#include <glm/glm.hpp>

namespace eo
{
Light::Light(const Shader& shader,
             const glm::vec3& direction,
             const glm::vec3& ambient_color,
             const glm::vec3& diffuse_color,
             const glm::vec3& specular_color)
    : _shader(shader)
{
    _u_direction = _shader.uniform_location_for("u_light.direction");
    _u_ambient = _shader.uniform_location_for("u_light.ambient_color");
    _u_diffuse = _shader.uniform_location_for("u_light.diffuse_color");
    _u_specular = _shader.uniform_location_for("u_light.specular_color");

    _shader.set_uniform_vec3(_u_direction, direction);
    _shader.set_uniform_vec3(_u_ambient, ambient_color);
    _shader.set_uniform_vec3(_u_diffuse, diffuse_color);
    _shader.set_uniform_vec3(_u_specular, specular_color);
}

void Light::set_direction(const glm::vec3& new_direction)
{
    _shader.set_uniform_vec3(_u_direction, new_direction);
}
} // namespace eo

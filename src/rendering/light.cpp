#include "light.h"
#include <glm/glm.hpp>

namespace eo
{
Light::Light(const Shader& shader,
             const glm::vec3& position,
             const glm::vec3& ambient_color,
             const glm::vec3& diffuse_color,
             const glm::vec3& specular_color)
    : _shader(shader)
{
    _u_position = _shader.uniform_location_for("u_light.position");
    _u_ambient = _shader.uniform_location_for("u_light.ambient_color");
    _u_diffuse = _shader.uniform_location_for("u_light.diffuse_color");
    _u_specular = _shader.uniform_location_for("u_light.specular_color");

    _shader.set_uniform_vec3(_u_position, position);
    _shader.set_uniform_vec3(_u_ambient, ambient_color);
    _shader.set_uniform_vec3(_u_diffuse, diffuse_color);
    _shader.set_uniform_vec3(_u_specular, specular_color);
}

void Light::set_position(const glm::vec3& new_position)
{
    _shader.set_uniform_vec3(_u_position, new_position);
}
} // namespace eo

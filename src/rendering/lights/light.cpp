#include "light.h"
#include <format>
#include <glm/glm.hpp>
#include <string>

namespace eo
{
Light::Light(const Shader& shader,
             const std::string& name,
             const glm::vec3& ambient_color,
             const glm::vec3& diffuse_color,
             const glm::vec3& specular_color)
    : _shader(shader)
{
    int u_ambient = _shader.uniform_location_for(std::format("{}.base.ambient_color", name));
    _shader.set_uniform_vec3(u_ambient, ambient_color);

    int u_diffuse = _shader.uniform_location_for(std::format("{}.base.diffuse_color", name));
    _shader.set_uniform_vec3(u_diffuse, diffuse_color);

    int u_specular = _shader.uniform_location_for(std::format("{}.base.specular_color", name));
    _shader.set_uniform_vec3(u_specular, specular_color);
}
} // namespace eo

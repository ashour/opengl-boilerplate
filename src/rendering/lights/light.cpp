#include "light.h"
#include <format>
#include <glm/glm.hpp>
#include <string>

namespace eo
{
Light::Light(Shader& shader,
             const std::string& name,
             const glm::vec3& ambient_color,
             const glm::vec3& diffuse_color,
             const glm::vec3& specular_color)
    : _shader(shader)
{
    _shader.set_uniform_vec3(std::format("{}.base.ambient_color", name), ambient_color);
    _shader.set_uniform_vec3(std::format("{}.base.diffuse_color", name), diffuse_color);
    _shader.set_uniform_vec3(std::format("{}.base.specular_color", name), specular_color);
}
} // namespace eo

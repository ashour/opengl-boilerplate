#include "point_light.h"
#include <format>
#include <glm/glm.hpp>
#include <string>

namespace eo
{
PointLight::PointLight(Shader& shader,
                       const std::string& name,
                       const glm::vec3& ambient_color,
                       const glm::vec3& diffuse_color,
                       const glm::vec3& specular_color,
                       const glm::vec3& position,
                       const float constant,
                       const float linear,
                       const float quadratic)
    : Light{shader, name, ambient_color, diffuse_color, specular_color}
{
    shader.set_uniform(std::format("{}.position", name), position);
    shader.set_uniform(std::format("{}.constant", name), constant);
    shader.set_uniform(std::format("{}.linear", name), linear);
    shader.set_uniform(std::format("{}.quadratic", name), quadratic);
}
} // namespace eo

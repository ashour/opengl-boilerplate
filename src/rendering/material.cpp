#include "material.h"
#include <glm/glm.hpp>

namespace eo
{
Material::Material(const Shader& shader,
                   const glm::vec3& ambient_color,
                   const glm::vec3& diffuse_color,
                   const glm::vec3& specular_color,
                   const float shininess)
    : _shader(shader),
      _ambient_color{ambient_color},
      _diffuse_color{diffuse_color},
      _specular_color{specular_color},
      _shininess{shininess}
{
    _u_ambient = _shader.uniform_location_for("u_material.ambient_color");
    _u_diffuse = _shader.uniform_location_for("u_material.diffuse_color");
    _u_specular = _shader.uniform_location_for("u_material.specular_color");
    _u_shininess = _shader.uniform_location_for("u_material.shininess");
}

void Material::use() const
{
    _shader.set_uniform_vec3(_u_ambient, _ambient_color);
    _shader.set_uniform_vec3(_u_diffuse, _diffuse_color);
    _shader.set_uniform_vec3(_u_specular, _specular_color);
    _shader.set_uniform_1f(_u_shininess, _shininess);
}
} // namespace eo

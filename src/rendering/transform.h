#pragma once

#include <glm/glm.hpp>

namespace eo
{
class Transform
{
  public:
    Transform();

    void position(const glm::vec3& position);
    void rotation(const float angle, const glm::vec3& axis);
    void scale(const glm::vec3& scale);

    const glm::mat4& matrix();

  private:
    glm::vec3 _position{};
    float _rotation_angle{};
    glm::vec3 _rotation_axis{};
    glm::vec3 _scale{1.0f};

    glm::mat4 _model_matrix{1.0f};
    bool _dirty{true};

    void update_matrix();
};
} // namespace eo

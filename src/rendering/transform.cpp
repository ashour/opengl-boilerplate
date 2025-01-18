#include "transform.h"
#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace eo
{
Transform::Transform() {}

void Transform::position(const glm::vec3& position)
{
    _dirty = true;
    _position = position;
}

void Transform::rotation(const float angle, const glm::vec3& axis)
{
    _dirty = true;
    _rotation_angle = angle;
    _rotation_axis = axis;
}

void Transform::scale(const glm::vec3& scale)
{
    _dirty = true;
    _scale = scale;
}

const glm::mat4& Transform::matrix()
{
    if (_dirty)
    {
        update_matrix();
    }
    return _model_matrix;
}

void Transform::update_matrix()
{
    _model_matrix = glm::mat4{1.0f};
    _model_matrix = glm::translate(_model_matrix, _position);
    _model_matrix = glm::rotate(_model_matrix, _rotation_angle, _rotation_axis);
    _model_matrix = glm::scale(_model_matrix, _scale);

    _dirty = false;
}

} // namespace eo

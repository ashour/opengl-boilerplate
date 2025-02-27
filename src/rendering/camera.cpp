#include "camera.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace eo
{
Camera::Camera(const float aspect_ratio)
    : _projection{glm::perspective(glm::radians(_fov), aspect_ratio, _near_plane, _far_plane)}
{
}

void Camera::move(Move direction, float delta_time)
{
    switch (direction)
    {
    case Move::forward:
        _position += _front * _speed * delta_time;
        break;
    case Move::back:
        _position -= _front * _speed * delta_time;
        break;
    case Move::left:
        _position -= glm::normalize(glm::cross(_front, _world_up)) * _speed * delta_time;
        break;
    case Move::right:
        _position += glm::normalize(glm::cross(_front, _world_up)) * _speed * delta_time;
        break;
    case Move::up:
        _position.y += _speed * delta_time;
        break;
    case Move::down:
        _position.y -= _speed * delta_time;
        break;
    }
}

void Camera::look(glm::vec2 current_mouse_position, glm::vec2 last_mouse_position)
{
    float x_offset{current_mouse_position.x - last_mouse_position.x};
    float y_offset{last_mouse_position.y - current_mouse_position.y};

    _yaw += (x_offset * _look_sensitivity);
    _pitch += (y_offset * _look_sensitivity);

    _pitch = glm::clamp(_pitch, _min_pitch, _max_pitch);

    glm::vec3 direction{cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
                        sin(glm::radians(_pitch)),
                        sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))};

    _front = glm::normalize(direction);
}

glm::mat4 Camera::view() const { return glm::lookAt(_position, _position + _front, _world_up); }
} // namespace eo

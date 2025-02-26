#pragma once

#include <glm/glm.hpp>

namespace eo
{
enum class Move
{
    forward,
    back,
    left,
    right,
    up,
    down,
};

class Camera
{
  public:
    explicit Camera(const float aspect_ratio);

    void move(Move direction, float delta_time);
    void look(glm::vec2 current_mouse_position, glm::vec2 last_mouse_position);

    glm::vec3 position() const { return _position; }
    glm::vec3 front() const { return _front; }
    glm::mat4 projection() const { return _projection; }
    glm::mat4 view() const;

  private:
    float _fov{45.0};
    float _near_plane{0.1f};
    float _far_plane{200.0f};
    glm::mat4 _projection{};

    float _speed{5.0f};
    glm::vec3 _position{0.0f, 5.0f, 30.0f};
    glm::vec3 _front{0.0f, 0.0f, -1.0f};
    glm::vec3 _world_up{0.0f, 1.0f, 0.0f};

    float _look_sensitivity{0.1f};
    const float _min_pitch{-89.0f};
    const float _max_pitch{89.0f};
    float _pitch{};
    float _yaw{-90.0f};
};
} // namespace eo

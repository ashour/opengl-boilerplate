#pragma once

#include <glm/glm.hpp>

namespace eo
{
enum class Strafe
{
    forward,
    back,
    left,
    right
};

class Camera
{
  public:
    explicit Camera(const float aspect_ratio);

    void strafe(Strafe direction, float delta_time);
    void look(glm::vec2 current_mouse_position, glm::vec2 last_mouse_position);

    glm::vec3 position() const { return _position; }
    glm::mat4 projection() const { return _projection; }
    glm::mat4 view() const;

  private:
    float _fov{45.0};
    float _near_plane{0.1f};
    float _far_plane{100.0f};
    glm::mat4 _projection{};

    float _speed{2.5f};
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

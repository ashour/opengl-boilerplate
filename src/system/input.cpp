#include "input.h"
#include <GLFW/glfw3.h>
#include <array>
#include <functional>

namespace eo
{
std::array<bool, Input::KEYS_SIZE> Input::_keys_pressed{};
std::array<bool, Input::KEYS_SIZE> Input::_keys_just_pressed{};
std::array<bool, Input::MOUSE_BUTTONS_SIZE> Input::_mouse_buttons{};
std::function<void(glm::vec2, glm::vec2)> Input::_on_mouse_moved{};

glm::vec2 Input::_last_mouse_position{};
glm::vec2 Input::_current_mouse_position{};
bool Input::_is_first_mouse_movement{true};

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key < 0 || key >= KEYS_SIZE)
    {
        return;
    }

    if (action == GLFW_PRESS)
    {
        _keys_pressed[key] = true;
        _keys_just_pressed[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        _keys_pressed[key] = false;
    }
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button < 0 || button >= MOUSE_BUTTONS_SIZE)
    {
        return;
    }

    if (action == GLFW_PRESS)
    {
        _mouse_buttons[button] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        _mouse_buttons[button] = false;
    }
}

bool Input::mouse_button_pressed(MouseButton button)
{
    return _mouse_buttons[static_cast<int>(button)];
}

bool Input::key_pressed(Key action) { return _keys_pressed[static_cast<int>(action)]; }

bool Input::key_just_pressed(Key action)
{
    int key = static_cast<int>(action);
    if (_keys_just_pressed[key])
    {
        _keys_just_pressed[key] = false;
        return true;
    }
    return false;
}

void Input::register_mouse_move_handler(std::function<void(glm::vec2, glm::vec2)> on_mouse_moved)
{
    _on_mouse_moved = on_mouse_moved;
}

void Input::mouse_callback(GLFWwindow* window, double x_position, double y_position)
{
    if (_is_first_mouse_movement)
    {
        _last_mouse_position.x = x_position;
        _last_mouse_position.y = y_position;
        _is_first_mouse_movement = false;
    }

    _current_mouse_position.x = x_position;
    _current_mouse_position.y = y_position;

    if (_on_mouse_moved)
    {
        _on_mouse_moved(_current_mouse_position, _last_mouse_position);
    }

    _last_mouse_position = _current_mouse_position;
}
} // namespace eo

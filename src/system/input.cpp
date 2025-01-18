#include "input.h"
#include <GLFW/glfw3.h>
#include <array>

std::array<bool, Input::KEYS_SIZE> Input::_keys{};

void (*Input::_on_mouse_moved)(glm::vec2, glm::vec2){};
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
        _keys[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        _keys[key] = false;
    }
}

bool Input::action_pressed(Action action) { return _keys[static_cast<int>(action)]; }

void Input::register_mouse_move_handler(void (*on_mouse_moved)(glm::vec2, glm::vec2))
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

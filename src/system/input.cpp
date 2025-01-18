#include "input.h"
#include <GLFW/glfw3.h>
#include <array>

std::array<bool, Input::KEYS_SIZE> Input::_keys{};

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

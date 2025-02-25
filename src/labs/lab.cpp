#include "lab.h"
#include "system/input.h"
#include "system/time.h"

namespace eo
{
bool Lab::_is_moving{false};

void Lab::toggle_movement()
{
    if (Input::key_just_pressed(Key::tab))
    {
        _is_moving = !_is_moving;
    }
}

void Lab::register_mouse_look(Camera& camera)
{
    Input::register_mouse_move_handler(
        [&camera](auto current_mouse_position, auto last_mouse_position)
        {
            if (_is_moving)
            {
                camera.look(current_mouse_position, last_mouse_position);
            }
        });
}

void Lab::strafe(Camera& camera)
{
    if (!_is_moving)
    {
        return;
    }

    if (Input::key_pressed(Key::w))
    {
        camera.strafe(Strafe::forward, Time::delta_time());
    }
    if (Input::key_pressed(Key::s))
    {
        camera.strafe(Strafe::back, Time::delta_time());
    }
    if (Input::key_pressed(Key::a))
    {
        camera.strafe(Strafe::left, Time::delta_time());
    }
    if (Input::key_pressed(Key::d))
    {
        camera.strafe(Strafe::right, Time::delta_time());
    }
}

} // namespace eo

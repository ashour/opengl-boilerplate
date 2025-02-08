#include "lab.h"
#include "system/input.h"
#include "system/time.h"

namespace eo
{

void Lab::register_mouse_look_on_hold_rmb(Camera& camera)
{
    Input::register_mouse_move_handler(
        [&camera](auto current_mouse_position, auto last_mouse_position)
        {
            if (Input::mouse_button_pressed(MouseButton::right))
            {
                camera.look(current_mouse_position, last_mouse_position);
            }
        });
}

void Lab::wasd_move_on_hold_rmb(Camera& camera)
{
    if (!Input::mouse_button_pressed(MouseButton::right))
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

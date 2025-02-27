#include "lab.h"
#include "system/input.h"
#include "system/time.h"
#include <format>

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

void Lab::strafe_and_fly(Camera& camera)
{
    if (!_is_moving)
    {
        return;
    }

    if (Input::key_pressed(Key::w))
    {
        camera.move(Move::forward, Time::delta_time());
    }
    else if (Input::key_pressed(Key::s))
    {
        camera.move(Move::back, Time::delta_time());
    }

    if (Input::key_pressed(Key::a))
    {
        camera.move(Move::left, Time::delta_time());
    }
    else if (Input::key_pressed(Key::d))
    {
        camera.move(Move::right, Time::delta_time());
    }

    if (Input::key_pressed(Key::e))
    {
        camera.move(Move::up, Time::delta_time());
    }
    else if (Input::key_pressed(Key::q))
    {
        camera.move(Move::down, Time::delta_time());
    }
}

void Lab::movement_help_ui(UI& ui)
{
    ui.begin_window(std::format("Movement controls ({})", _is_moving ? "On" : "Off"));
    ui.text("[Tab] to toggle movement controls on/off.");
    ui.text("[W][A][S][D] to strafe.");
    ui.text("Mouse to look.");
    ui.text("[E] to fly up. [Q] to fly down.");
    ui.end_window();
}

} // namespace eo

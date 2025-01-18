#include "time.h"

namespace eo
{
float Time::_last_frame_time{};
float Time::_current_frame_time{};
float Time::_delta_time{};

void Time::update()
{
    _current_frame_time = glfwGetTime();
    _delta_time = _current_frame_time - _last_frame_time;
    _last_frame_time = _current_frame_time;
}
} // namespace eo

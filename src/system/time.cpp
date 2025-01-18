#include "time.h"

namespace eo
{
float Time::_last_frame_time{};
float Time::_delta_time{};

void Time::update()
{
    float current_frame_time = glfwGetTime();
    _delta_time = current_frame_time - _last_frame_time;
    _last_frame_time = current_frame_time;
}
} // namespace eo

#pragma once

#include <GLFW/glfw3.h>

namespace eo
{
class Time
{
  public:
    Time() = delete;

    static void update();
    static float delta_time() { return _delta_time; }

  private:
    static float _last_frame_time;
    static float _delta_time;
};
} // namespace eo

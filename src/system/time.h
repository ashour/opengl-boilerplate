#pragma once

namespace eo
{
class Time
{
  public:
    Time() = delete;

    static void update();
    static float current_time() { return _current_frame_time; }
    static float delta_time() { return _delta_time; }

  private:
    static float _current_frame_time;
    static float _last_frame_time;
    static float _delta_time;
};
} // namespace eo

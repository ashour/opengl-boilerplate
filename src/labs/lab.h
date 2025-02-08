#pragma once

#include "rendering/camera.h"
#include "system/window.h"

namespace eo
{
class Lab
{
  public:
    Lab(const Window& window) : _window{window} {}
    virtual ~Lab() {}

    virtual void on_update() {}
    virtual void on_render() {}

  protected:
    const Window& _window;

    static void register_mouse_look_on_hold_rmb(Camera& camera);
    static void wasd_move_on_hold_rmb(Camera& camera);
};
} // namespace eo

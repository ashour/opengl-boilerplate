#pragma once

#include "rendering/camera.h"
#include "system/ui.h"
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
    virtual void on_ui_render(UI& ui) {}

  protected:
    const Window& _window;

    static bool _is_moving;

    static void toggle_movement();
    static void register_mouse_look(Camera& camera);
    static void strafe(Camera& camera);

    static void movement_help_ui(UI& ui);
};
} // namespace eo

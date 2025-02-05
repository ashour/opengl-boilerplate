#pragma once

#include "system/window.h"

namespace eo
{
class Lab
{
  public:
    Lab(const Window& window) : _window{window} {}
    virtual ~Lab() {}

    virtual void OnUpdate() {}
    virtual void OnRender() {}

  protected:
    const Window& _window;
};
} // namespace eo

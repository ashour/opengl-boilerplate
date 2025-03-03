#pragma once

#include "labs/lab.h"
#include "system/window.h"

namespace eo
{
class InstancingLab : public Lab
{
  public:
    InstancingLab(const Window& window);
    ~InstancingLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    Lab* _sub_lab;

    enum class SubLab
    {
        quads_instanced,
        quads_instanced_arrays,
        asteroids,
        asteroids_instanced,
    };
    SubLab _selected_lab;
};
} // namespace eo

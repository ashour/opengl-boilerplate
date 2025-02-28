#pragma once

#include "labs/lab.h"
#include "system/window.h"

namespace eo
{
class GeometryShadersLab : public Lab
{
  public:
    GeometryShadersLab(const Window& window);
    ~GeometryShadersLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    Lab* _sub_lab;

    enum class SubLab
    {
        points,
        house,
        explode,
        normals
    };
    SubLab _selected_lab;
};
} // namespace eo

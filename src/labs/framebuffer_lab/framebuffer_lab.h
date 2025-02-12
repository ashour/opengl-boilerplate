#pragma once

#include "labs/lab.h"

namespace eo
{
class FramebufferLab : public Lab
{
  public:
    FramebufferLab(const Window& window);
    ~FramebufferLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    Lab* _sub_lab;

    enum class SubLab
    {
        render_texture_to_world,
        render_texture_to_screen,
        negative,
        greyscale,
        weighted_greyscale,
        sharpen,
        blur,
        edge_detection,
    };
    SubLab _selected_lab;
};
} // namespace eo

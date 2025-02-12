#include "fbl_render_texture_to_screen.h"
#include "fbl_render_texture_to_world.h"
#include "framebuffer_lab.h"
#include "labs/registry/lab_registry.h"

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab("Framebuffers",
                                  [](eo::Window& window) -> eo::Lab*
                                  { return new eo::FramebufferLab(window); });
    return true;
}();
} // namespace

namespace eo
{

FramebufferLab::FramebufferLab(const Window& window) : Lab(window)
{
    std::vector<float> kernel = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
    _sub_lab =
        new Fbl_RenderTextureToScreen(_window, "resources/shaders/kernel.frag", true, &kernel);
    _selected_lab = SubLab::sharpen;
}

FramebufferLab::~FramebufferLab() { delete _sub_lab; }

void FramebufferLab::on_update() { _sub_lab->on_update(); }

void FramebufferLab::on_render() { _sub_lab->on_render(); }

void FramebufferLab::on_ui_render(UI& ui)
{
    ui.begin_window("Framebuffers");
    if (ui.radio_button("Render texture in world",
                        _selected_lab == SubLab::render_texture_to_world))
    {
        delete _sub_lab;
        _sub_lab = new Fbl_RenderTextureToWorld(_window);
        _selected_lab = SubLab::render_texture_to_world;
    }
    if (ui.radio_button("Render texture to screen (RTS)",
                        _selected_lab == SubLab::render_texture_to_screen))
    {
        delete _sub_lab;
        _sub_lab = new Fbl_RenderTextureToScreen(_window, "resources/shaders/unlit_texture.frag");
        _selected_lab = SubLab::render_texture_to_screen;
    }
    if (ui.radio_button("RTS: Negative", _selected_lab == SubLab::negative))
    {
        delete _sub_lab;
        _sub_lab = new Fbl_RenderTextureToScreen(_window, "resources/shaders/negative.frag");
        _selected_lab = SubLab::negative;
    }
    if (ui.radio_button("RTS: Greyscale", _selected_lab == SubLab::greyscale))
    {
        delete _sub_lab;
        _sub_lab = new Fbl_RenderTextureToScreen(_window, "resources/shaders/greyscale.frag");
        _selected_lab = SubLab::greyscale;
    }
    if (ui.radio_button("RTS: Weighted Greyscale", _selected_lab == SubLab::weighted_greyscale))
    {
        delete _sub_lab;
        _sub_lab =
            new Fbl_RenderTextureToScreen(_window, "resources/shaders/weighted_greyscale.frag");
        _selected_lab = SubLab::weighted_greyscale;
    }
    if (ui.radio_button("RTS: (Kernel) Sharpen", _selected_lab == SubLab::sharpen))
    {
        delete _sub_lab;
        std::vector<float> kernel = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
        _sub_lab =
            new Fbl_RenderTextureToScreen(_window, "resources/shaders/kernel.frag", true, &kernel);
        _selected_lab = SubLab::sharpen;
    }
    ui.end_window();
}

} // namespace eo

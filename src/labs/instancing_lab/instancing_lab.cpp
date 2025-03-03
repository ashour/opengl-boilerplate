#include "il_quads_instanced.h"
#include "il_quads_instanced_arrays.h"
#include "instancing_lab.h"
#include "labs/registry/lab_registry.h"

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab(
        "Instancing", [](eo::Window& window) -> eo::Lab* { return new eo::InstancingLab(window); });
    return true;
}();
} // namespace

namespace eo
{
InstancingLab::InstancingLab(const Window& window) : Lab(window)
{
    _sub_lab = new IL_QuadsInstanced(_window);
    _selected_lab = SubLab::quads_instanced;
}

InstancingLab::~InstancingLab() { delete _sub_lab; }

void InstancingLab::on_update() { _sub_lab->on_update(); }

void InstancingLab::on_render() { _sub_lab->on_render(); }

void InstancingLab::on_ui_render(UI& ui)
{
    ui.begin_window("Instancing");
    if (ui.radio_button("Quads (instanced)", _selected_lab == SubLab::quads_instanced))
    {
        delete _sub_lab;
        _sub_lab = new IL_QuadsInstanced(_window);
        _selected_lab = SubLab::quads_instanced;
    }
    if (ui.radio_button("Quads (instance arrays)", _selected_lab == SubLab::quads_instanced_arrays))
    {
        delete _sub_lab;
        _sub_lab = new IL_QuadsInstancedArrays(_window);
        _selected_lab = SubLab::quads_instanced_arrays;
    }
    ui.end_window();

    _sub_lab->on_ui_render(ui);
}

} // namespace eo

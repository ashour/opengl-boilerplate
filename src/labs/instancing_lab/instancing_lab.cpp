#include "il_quads_instanced.h"
#include "il_quads_instanced_arrays.h"
#include "instancing_lab.h"
#include "labs/instancing_lab/il_asteroids.h"
#include "labs/instancing_lab/il_asteroids_instanced.h"
#include "labs/registry/lab_registry.h"

namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab("12. Instancing",
                                  [](eo::Window& window) -> eo::Lab*
                                  { return new eo::InstancingLab(window); });
    return true;
}();
} // namespace

namespace eo
{
InstancingLab::InstancingLab(const Window& window) : Lab(window)
{
    _sub_lab = new IL_Asteroids(_window);
    _selected_lab = SubLab::asteroids;
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
    if (ui.radio_button("Quads (instanced arrays)",
                        _selected_lab == SubLab::quads_instanced_arrays))
    {
        delete _sub_lab;
        _sub_lab = new IL_QuadsInstancedArrays(_window);
        _selected_lab = SubLab::quads_instanced_arrays;
    }
    if (ui.radio_button("Asteroids (individual)", _selected_lab == SubLab::asteroids))
    {
        delete _sub_lab;
        _sub_lab = new IL_Asteroids(_window);
        _selected_lab = SubLab::asteroids;
    }
    if (ui.radio_button("Asteroids (instanced)", _selected_lab == SubLab::asteroids_instanced))
    {
        delete _sub_lab;
        _sub_lab = new IL_AsteroidsInstanced(_window);
        _selected_lab = SubLab::asteroids_instanced;
    }
    ui.end_window();

    _sub_lab->on_ui_render(ui);
}

} // namespace eo

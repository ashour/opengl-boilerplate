#include "config.h"
#include "geometry_shaders_lab.h"
#include "labs/geometry_shaders_lab/gsl_house.h"
#include "labs/geometry_shaders_lab/gsl_points.h"
#include "labs/registry/lab_registry.h"
#include "system/metrics.h"
namespace
{
const bool registered = []()
{
    eo::LabRegistry::register_lab("Geometry Shaders",
                                  [](eo::Window& window) -> eo::Lab*
                                  { return new eo::GeometryShadersLab(window); });
    return true;
}();
} // namespace

namespace eo
{
GeometryShadersLab::GeometryShadersLab(const Window& window) : Lab(window)
{
    _sub_lab = new Gsl_Points(_window);
    _selected_lab = SubLab::points;
}

GeometryShadersLab::~GeometryShadersLab() { delete _sub_lab; }

void GeometryShadersLab::on_update() { _sub_lab->on_update(); }

void GeometryShadersLab::on_render() { _sub_lab->on_render(); }

void GeometryShadersLab::on_ui_render(UI& ui)
{
    ui.begin_window("Geometry Shaders");
    if (ui.radio_button("Emit points (pass-through)", _selected_lab == SubLab::points))
    {
        delete _sub_lab;
        _sub_lab = new Gsl_Points(_window);
        _selected_lab = SubLab::points;
    }
    if (ui.radio_button("Houses", _selected_lab == SubLab::house))
    {
        delete _sub_lab;
        _sub_lab = new Gsl_House(_window);
        _selected_lab = SubLab::house;
    }
    ui.end_window();
}

} // namespace eo

#pragma once

#include "labs/lab.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class GeometryShadersLab : public Lab
{
  public:
    GeometryShadersLab(const Window& window);
    ~GeometryShadersLab();

    void on_render() override;

  private:
    unsigned int _points_vao;

    std::unique_ptr<Shader> _points_shader;
};
} // namespace eo

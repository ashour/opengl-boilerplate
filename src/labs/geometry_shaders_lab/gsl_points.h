#pragma once

#include "labs/instancing_lab/il_instanced_quads.h"
#include "labs/lab.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class Gsl_Points : public Lab
{
  public:
    Gsl_Points(const Window& window);

    void on_render() override;

  private:
    std::unique_ptr<VertexArray> _vertex_array;

    std::unique_ptr<Shader> _points_shader;
};
} // namespace eo

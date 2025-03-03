#pragma once

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
    ~Gsl_Points();

    void on_render() override;

  private:
    unsigned int _points_vao;
    unsigned int _points_vbo;

    std::unique_ptr<Shader> _points_shader;
};
} // namespace eo

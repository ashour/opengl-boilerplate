#pragma once

#include "labs/lab.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class Gsl_House : public Lab
{
  public:
    Gsl_House(const Window& window);
    ~Gsl_House();

    void on_render() override;

  private:
    unsigned int _points_vao;

    std::unique_ptr<Shader> _points_shader;
};
} // namespace eo

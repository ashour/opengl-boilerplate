#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/model.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class Gsl_Explode : public Lab
{
  public:
    Gsl_Explode(const Window& window);
    ~Gsl_Explode();

    void on_render() override;

  private:
    std::unique_ptr<Camera> _camera;

    std::unique_ptr<Shader> _explode_shader;
    std::unique_ptr<Model> _backpack;
};
} // namespace eo

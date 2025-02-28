#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/model.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class Gsl_VisualizeNormals : public Lab
{
  public:
    Gsl_VisualizeNormals(const Window& window);
    ~Gsl_VisualizeNormals();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    std::unique_ptr<Camera> _camera;

    std::unique_ptr<Shader> _backpack_shader;
    std::unique_ptr<Model> _backpack;

    std::unique_ptr<Shader> _visualize_normals_shader;
};
} // namespace eo

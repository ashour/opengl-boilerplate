#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class AntialiasingLab : public Lab
{
  public:
    AntialiasingLab(const Window& window);
    ~AntialiasingLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _shader;

    int _u_view_matrix;

    std::unique_ptr<Mesh> _cube;
};
} // namespace eo

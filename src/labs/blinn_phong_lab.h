#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/shader.h"
#include "rendering/texture.h"
#include "rendering/vertex_array.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class BlinnPhongLab : public Lab
{
  public:
    BlinnPhongLab(const Window& window);
    ~BlinnPhongLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    std::unique_ptr<Camera> _camera;
    std::unique_ptr<Shader> _shader;

    std::unique_ptr<VertexArray> _ground_vertex_array;
    Texture _ground_texture;

    bool _use_blinn{false};
};
} // namespace eo

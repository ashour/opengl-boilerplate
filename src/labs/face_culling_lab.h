#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class FaceCullingLab : public Lab
{
  public:
    FaceCullingLab(const Window& window);
    ~FaceCullingLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    enum class Cull
    {
        none = -1,
        front = GL_FRONT,
        back = GL_BACK,
        front_and_back = GL_FRONT_AND_BACK,
    };
    Cull _cull{Cull::back};

    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _shader;

    std::shared_ptr<Material> _mat_dirt;
    std::shared_ptr<Material> _mat_box;

    int _u_view_matrix;

    std::unique_ptr<Mesh> _cube;

    std::unique_ptr<Mesh> _ground;

    void culling_radio_button(UI& ui, const std::string& label, Cull type);
};
} // namespace eo

#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/lights/directional_light.h"
#include "rendering/lights/spot_light.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/model.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class ModelLab : public Lab
{
  public:
    ModelLab(const Window& window);

    void OnUpdate() override;
    void OnRender() override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _shader;
    std::unique_ptr<DirectionalLight> _directional_light;
    std::unique_ptr<SpotLight> _flash_light;

    std::shared_ptr<Material> _mat_dirt;

    int _u_view_matrix;
    int _u_view_position;

    std::unique_ptr<Mesh> _plane;
    std::unique_ptr<Model> _x_wing;
    std::unique_ptr<Model> _backpack;
};
} // namespace eo

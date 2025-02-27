#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/lights/directional_light.h"
#include "rendering/lights/spot_light.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <array>
#include <memory>

namespace eo
{
class PhongLab : public Lab
{
  public:
    PhongLab(const Window& window);
    ~PhongLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _shader;
    std::unique_ptr<DirectionalLight> _directional_light;
    std::unique_ptr<SpotLight> _flash_light;

    std::shared_ptr<Material> _mat_dirt;
    std::shared_ptr<Material> _mat_box;

    int _u_view_matrix;
    int _u_view_position;

    std::unique_ptr<Mesh> _cube;
    std::array<glm::vec3, 180> _cube_positions{};

    std::unique_ptr<Mesh> _ground;
};
} // namespace eo

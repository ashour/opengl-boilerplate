#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <array>
#include <memory>

namespace eo
{
class UnlitLab : public Lab
{
  public:
    UnlitLab(const Window& window);
    ~UnlitLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _shader;

    std::shared_ptr<Material> _mat_dirt;
    std::shared_ptr<Material> _mat_box;

    int _u_view_matrix;

    std::unique_ptr<Mesh> _cube;
    std::array<glm::vec3, 180> _cube_positions{};

    std::unique_ptr<Mesh> _ground;
};
} // namespace eo

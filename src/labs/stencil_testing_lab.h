#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>
#include <array>

namespace eo
{
class StencilTestingLab : public Lab
{
  public:
    StencilTestingLab(const Window& window);

    void on_update() override;
    void on_render() override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _render_shader;
    std::shared_ptr<Shader> _stencil_shader;

    std::shared_ptr<Material> _mat_dirt;
    std::shared_ptr<Material> _mat_box;

    std::unique_ptr<Mesh> _cube;
    std::array<glm::vec3, 180> _cube_positions{};

    std::unique_ptr<Mesh> _ground;
};
} // namespace eo

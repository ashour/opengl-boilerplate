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
class DepthTestingLab : public Lab
{
  public:
    DepthTestingLab(const Window& window);

    void on_update() override;
    void on_render() override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _shader;

    int _u_view_matrix;

    std::unique_ptr<Mesh> _cube;
    std::array<glm::vec3, 180> _cube_positions{};

    std::unique_ptr<Mesh> _ground;
};
} // namespace eo

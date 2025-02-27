#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class UniformBuffersLab : public Lab
{
  public:
    UniformBuffersLab(const Window& window);
    ~UniformBuffersLab();

    void on_render() override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _red_shader;
    std::shared_ptr<Shader> _blue_shader;
    std::shared_ptr<Shader> _green_shader;
    std::shared_ptr<Shader> _yellow_shader;

    std::unique_ptr<Mesh> _cube;
};
} // namespace eo

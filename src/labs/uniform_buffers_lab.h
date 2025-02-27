#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "rendering/uniform_buffer.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class UniformBuffersLab : public Lab
{
  public:
    UniformBuffersLab(const Window& window);
    ~UniformBuffersLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    std::unique_ptr<Camera> _camera;

    std::unique_ptr<UniformBuffer> _matrices_uniform_buffer;

    std::shared_ptr<Shader> _red_shader;
    std::shared_ptr<Shader> _blue_shader;
    std::shared_ptr<Shader> _green_shader;
    std::shared_ptr<Shader> _yellow_shader;

    std::unique_ptr<Mesh> _cube;
};
} // namespace eo

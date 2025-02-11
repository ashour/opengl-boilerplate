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
class FramebufferLab : public Lab
{
  public:
    FramebufferLab(const Window& window);

    void on_update() override;
    void on_render() override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _scene_shader;
    std::shared_ptr<Shader> _quad_shader;

    std::shared_ptr<Material> _mat_dirt;
    std::shared_ptr<Material> _mat_box;

    std::unique_ptr<Mesh> _quad;

    std::unique_ptr<Mesh> _cube;
    std::array<glm::vec3, 180> _cube_positions{};

    std::unique_ptr<Mesh> _ground;

    unsigned int _fbo;
    unsigned int _tex_color_buffer;

    void render_scene();
};
} // namespace eo

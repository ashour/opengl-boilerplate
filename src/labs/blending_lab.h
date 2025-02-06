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
class BlendingLab : public Lab
{
  public:
    BlendingLab(const Window& window);
    ~BlendingLab();

    void on_update() override;
    void on_render() override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _unlit_tex_shader;
    std::shared_ptr<Shader> _discard_transparent_tex_shader;
    std::shared_ptr<Shader> _unlit_alpha_texture;

    std::shared_ptr<Material> _mat_dirt;
    std::shared_ptr<Material> _mat_box;
    std::shared_ptr<Material> _mat_grass;
    std::shared_ptr<Material> _mat_window;

    int _u_view_matrix;

    std::unique_ptr<Mesh> _cube;
    std::array<glm::vec3, 180> _cube_positions{};

    std::unique_ptr<Mesh> _ground;

    std::unique_ptr<Mesh> _grass;
    std::unique_ptr<Mesh> _transparent_window;
    std::array<glm::vec3, 4> _window_positions{
        glm::vec3{-10.0f, 4.0f, 0.25f},
        glm::vec3{-10.5f, 4.5f, 0.0f},
        glm::vec3{-7.5f, 2.0f, 0.0f},
        glm::vec3{-8.0f, 1.5f, -0.25f},
    };
};
} // namespace eo

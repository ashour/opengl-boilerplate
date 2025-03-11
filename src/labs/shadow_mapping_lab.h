#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/lights/directional_light.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <array>
#include <memory>

namespace eo
{
class ShadowMappingLab : public Lab
{
  public:
    ShadowMappingLab(const Window& window);
    ~ShadowMappingLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _scene_shader;
    std::unique_ptr<DirectionalLight> _directional_light;

    bool _rotate_light{false};

    std::shared_ptr<Material> _mat_dirt;
    std::shared_ptr<Material> _mat_box;

    int _u_view_matrix;
    int _u_view_position;

    std::unique_ptr<Mesh> _cube;
    std::array<glm::vec3, 180> _cube_positions{};

    std::unique_ptr<Mesh> _ground;

    const unsigned int SHADOW_WIDTH{4096};
    const unsigned int SHADOW_HEIGHT{4096};
    unsigned int _depth_map_fbo;
    unsigned int _depth_map_tex;

    std::unique_ptr<Shader> _simple_depth_shader;

    bool _render_debug_quad{false};
    std::unique_ptr<Mesh> _debug_quad;
    std::unique_ptr<Shader> _debug_quad_shader;

    void render_scene(Shader& shader, float use_materials, float time);
};
} // namespace eo

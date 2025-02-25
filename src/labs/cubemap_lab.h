#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/model.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <array>
#include <memory>

namespace eo
{
class CubemapLab : public Lab
{
  public:
    CubemapLab(const Window& window);
    ~CubemapLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _skybox_reflection_shader;
    std::shared_ptr<Shader> _skybox_refraction_shader;

    float _refractive_index{1.00f};

    std::shared_ptr<Material> _mat_box;

    std::unique_ptr<Mesh> _cube;
    std::array<glm::vec3, 180> _cube_positions{};

    std::unique_ptr<Model> _backpack;

    std::unique_ptr<Mesh> _skybox;
    std::shared_ptr<Shader> _skybox_shader;
    unsigned int _skybox_texture;

    unsigned int load_cubemap(std::vector<const std::string>& face_texture_filepaths);
};
} // namespace eo

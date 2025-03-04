#pragma once

#include "labs/lab.h"
#include "rendering/camera.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class AntialiasingLab : public Lab
{
  public:
    AntialiasingLab(const Window& window);
    ~AntialiasingLab();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    std::unique_ptr<Camera> _camera;

    std::shared_ptr<Shader> _scene_shader;
    std::unique_ptr<Mesh> _cube;

    std::unique_ptr<Mesh> _screen_quad;
    std::shared_ptr<Shader> _screen_shader;

    unsigned int _multisampled_fbo;
    unsigned int _color_buffer_multisampled_texture;
    unsigned int _rbo;
    unsigned int _intermediate_fbo;
    unsigned int _screen_texture;
};
} // namespace eo

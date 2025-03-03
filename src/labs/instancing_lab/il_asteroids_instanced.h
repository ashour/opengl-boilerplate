#pragma once

#include "labs/lab.h"
#include "rendering/model.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class IL_AsteroidsInstanced : public Lab
{
  public:
    IL_AsteroidsInstanced(const Window& window);
    ~IL_AsteroidsInstanced();

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    std::unique_ptr<Camera> _camera;

    std::unique_ptr<Shader> _planet_shader;
    std::unique_ptr<Model> _planet;

    unsigned int _rock_instance_vbo;

    std::unique_ptr<Shader> _rock_shader;
    static constexpr unsigned int ROCK_COUNT{16000};
    glm::mat4 _model_matrices[ROCK_COUNT];
    std::unique_ptr<Model> _rock;
};
} // namespace eo

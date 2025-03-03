#pragma once

#include "labs/lab.h"
#include "rendering/model.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class IL_Asteroids : public Lab
{
  public:
    IL_Asteroids(const Window& window);

    void on_update() override;
    void on_render() override;
    void on_ui_render(UI& ui) override;

  private:
    std::unique_ptr<Camera> _camera;
    std::unique_ptr<Shader> _shader;

    std::unique_ptr<Model> _planet;

    static constexpr unsigned int ROCK_COUNT{16000};
    glm::mat4 _model_matrices[ROCK_COUNT];
    std::unique_ptr<Model> _rock;
};
} // namespace eo

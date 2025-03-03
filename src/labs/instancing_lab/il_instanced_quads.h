#pragma once

#include "labs/lab.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class IL_InstancedQuads : public Lab
{
  public:
    IL_InstancedQuads(const Window& window);
    ~IL_InstancedQuads();

    void on_render() override;

  private:
    unsigned int _quad_vao;
    unsigned int _quad_vbo;

    std::unique_ptr<Shader> _shader;
};
} // namespace eo

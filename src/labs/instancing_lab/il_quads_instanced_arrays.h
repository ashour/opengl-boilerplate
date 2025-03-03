#pragma once

#include "labs/lab.h"
#include "rendering/shader.h"
#include "rendering/vertex_array.h"
#include "system/window.h"
#include <memory>

namespace eo
{
class IL_QuadsInstancedArrays : public Lab
{
  public:
    IL_QuadsInstancedArrays(const Window& window);

    void on_render() override;

  private:
    std::unique_ptr<VertexArray> _vertex_array;

    std::unique_ptr<Shader> _shader;
};
} // namespace eo

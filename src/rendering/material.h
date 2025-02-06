#pragma once

#include "rendering/shader.h"
#include "rendering/texture.h"
#include <memory>

namespace eo
{
class Material
{
  public:
    Material(const std::vector<std::shared_ptr<Texture>>& textures, const float shininess);

    const std::vector<std::shared_ptr<Texture>>& textures() const { return _textures; }
    constexpr float shininess() const { return _shininess; }

    void bind(Shader& shader);
    void unbind(Shader& shader);

  private:
    static std::unique_ptr<Texture> _black_pixel;

    std::vector<std::shared_ptr<Texture>> _textures;
    const float _shininess{};
};
} // namespace eo

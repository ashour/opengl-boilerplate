#pragma once

#include <glad/glad.h>
#include <string>

namespace eo
{
enum class TextureUnit
{
    TEXUNIT0 = GL_TEXTURE0
};

class Texture
{
  public:
    explicit Texture(const std::string& file_path);
    ~Texture();

    void bind(TextureUnit unit) const;

  private:
    unsigned int _id{0};
};
} // namespace eo

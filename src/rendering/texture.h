#pragma once

#include <glad/glad.h>
#include <string>

namespace eo
{
enum class TextureUnit
{
    TEXUNIT0 = GL_TEXTURE0,
    TEXUNIT1 = GL_TEXTURE1
};

enum class Format
{
    RGB = GL_RGB,
    RGBA = GL_RGBA
};

class Texture
{
  public:
    Texture(const std::string& file_path, const Format& format);
    Texture(const unsigned char* color_data,
            const int width,
            const int height,
            const Format& format);
    ~Texture();

    void bind(TextureUnit unit) const;

  private:
    void create_bind_init();

    unsigned int _id{0};
};
} // namespace eo

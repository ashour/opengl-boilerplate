// clang-format off
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// clang-format on
#include "lib/opengl_debug.h"
#include "texture.h"

namespace eo
{

Texture::Texture(const std::string& file_path, const Format& format)
{
    gldc(glGenTextures(1, &_id));
    gldc(glBindTexture(GL_TEXTURE_2D, _id));

    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    stbi_set_flip_vertically_on_load(true);

    int width;
    int height;
    int channel_count;
    unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &channel_count, 0);
    if (data)
    {
        gldc(glTexImage2D(GL_TEXTURE_2D,
                          0,
                          GL_RGB,
                          width,
                          height,
                          0,
                          static_cast<GLenum>(format),
                          GL_UNSIGNED_BYTE,
                          data));
        gldc(glGenerateMipmap(GL_TEXTURE_2D));
    }
    else
    {
        EO_LOG_ERROR("Failed to load texture " << file_path);
    }
    stbi_image_free(data);
}

Texture::~Texture() { gldc(glDeleteTextures(1, &_id)); }

void Texture::bind(TextureUnit unit) const
{
    gldc(glActiveTexture(static_cast<GLenum>(unit)));
    gldc(glBindTexture(GL_TEXTURE_2D, _id));
}
} // namespace eo

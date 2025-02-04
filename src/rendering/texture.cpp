#include "lib/opengl_debug.h"
#include "texture.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace eo
{
Texture::Texture(const std::string& type, const std::string& file_path)
    : type{type}, path{file_path}
{
    gldc(glGenTextures(1, &id));
    gldc(glBindTexture(GL_TEXTURE_2D, id));

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
        GLenum format;
        switch (channel_count)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            EO_LOG_ERROR("Unknown texture format!");
        }

        gldc(glTexImage2D(
            GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        gldc(glGenerateMipmap(GL_TEXTURE_2D));
    }
    else
    {
        EO_LOG_ERROR("Failed to load texture " << file_path);
    }
    stbi_image_free(data);
}

Texture::Texture(const void* color_data, const int width, const int height, const std::string& type)
    : type{type}
{
    gldc(glGenTextures(1, &id));
    gldc(glBindTexture(GL_TEXTURE_2D, id));

    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    gldc(glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, color_data));
    gldc(glGenerateMipmap(GL_TEXTURE_2D));
}

Texture::~Texture() { gldc(glDeleteTextures(1, &id)); }
} // namespace eo

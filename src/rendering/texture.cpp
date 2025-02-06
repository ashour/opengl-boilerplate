#include "rendering/rendering.h"
#include "texture.h"
#include <stb_image.h>

namespace eo
{
Texture::Texture(Type type, const std::string& file_path) : _type{type}, _path{file_path}
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

Texture::Texture(const void* color_data, const int width, const int height, const std::string& path)
    : _type(Type::solid_color), _path{path}
{
    gldc(glGenTextures(1, &_id));
    gldc(glBindTexture(GL_TEXTURE_2D, _id));

    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    gldc(glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, color_data));
    gldc(glGenerateMipmap(GL_TEXTURE_2D));
}

Texture::~Texture() { gldc(glDeleteTextures(1, &_id)); }

std::shared_ptr<Texture> Texture::no_specular()
{
    static std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture::Type::no_specular);
    return texture;
}

Texture::Texture(Type type) : _id{}, _type{type}, _path{} {}
} // namespace eo

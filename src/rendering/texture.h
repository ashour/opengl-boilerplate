#pragma once

#include <glad/glad.h>
#include <string>

namespace eo
{
class Texture
{
  public:
    Texture(const std::string& type, const std::string& file_path);
    Texture(const void* color_data, const int width, const int height, const std::string& type);
    ~Texture();

    unsigned int id;
    std::string type;
    std::string path;
    GLenum format;
};
} // namespace eo

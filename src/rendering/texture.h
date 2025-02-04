#pragma once

#include <glad/glad.h>
#include <memory>
#include <string>

namespace eo
{
class Texture
{
  public:
    Texture(const std::string& type, const std::string& file_path);
    Texture(const void* color_data,
            const int width,
            const int height,
            const std::string& type,
            const std::string& path = "");
    ~Texture();

    unsigned int id() const { return _id; }
    std::string type() const { return _type; }
    std::string path() const { return _path; }

    static std::shared_ptr<Texture> no_specular();

  private:
    Texture(std::string type);

    unsigned int _id;
    const std::string _type;
    const std::string _path;
};
} // namespace eo

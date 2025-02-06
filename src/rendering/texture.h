#pragma once

#include <memory>
#include <string>

namespace eo
{
class Texture
{
  public:
    enum class Type
    {
        solid_color = 0,
        diffuse,
        specular,
        no_specular,
    };

    Texture(const Type type, const std::string& file_path);
    Texture(const void* color_data,
            const int width,
            const int height,
            const std::string& path = "");
    Texture(Type type);
    ~Texture();

    unsigned int id() const { return _id; }
    Type type() const { return _type; }
    std::string path() const { return _path; }

    static std::shared_ptr<Texture> no_specular();

  private:
    unsigned int _id;
    const Type _type;
    const std::string _path;
};
} // namespace eo

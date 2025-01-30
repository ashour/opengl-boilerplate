#pragma once

#include "lib/opengl_debug.h"
#include <glad/glad.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace eo
{
class Shader
{
  public:
    Shader(const std::string& vertex_shader_filepath, const std::string& fragment_shader_filepath);
    ~Shader();

    void build();
    void use() const;
    static void unuse_all();

    int uniform_location_for(const std::string& variable);

    template <typename T>
    void set_uniform(const unsigned int location, const T& value) const
    {
        if constexpr (std::is_same_v<T, int>)
        {
            gldc(glUniform1i(location, value));
        }
        else if constexpr (std::is_same_v<T, unsigned int>)
        {
            gldc(glUniform1ui(location, value));
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            gldc(glUniform1f(location, value));
        }
        else if constexpr (std::is_same_v<T, glm::vec3>)
        {
            gldc(glUniform3f(location, value.x, value.y, value.z));
        }
        else if constexpr (std::is_same_v<T, glm::mat4>)
        {
            gldc(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
        }
        else
        {
            static_assert(always_false<T>, "Unsupported uniform type");
        }
    }

    template <typename T>
    void set_uniform(const std::string& name, const T& value)
    {
        set_uniform(uniform_location_for(name), value);
    }

  private:
    const std::string _vertex_shader_filepath;
    const std::string _fragment_shader_filepath;
    unsigned int _shader_program;

    unsigned int create_shader(const GLenum type,
                               const std::string& filepath,
                               char* log_buffer,
                               const size_t log_buffer_size) const;

    void delete_shader(const unsigned int shader) const;

    unsigned int create_program(const unsigned int vertex_shader,
                                const unsigned int fragment_shader,
                                char* log_buffer,
                                const size_t log_buffer_size) const;

    std::unordered_map<std::string, int> _uniform_locations;

    template <typename T>
    static constexpr bool always_false = false;
};
} // namespace eo

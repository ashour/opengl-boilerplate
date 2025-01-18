#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
  public:
    Shader(const std::string& vertex_shader_filepath, const std::string& fragment_shader_filepath);
    ~Shader();

    void build();
    void use() const;
    static void unuse_all();

    unsigned int uniform_location_for(const std::string& variable) const;

    void set_uniform_mat4(const unsigned int location, const glm::mat4& value) const;

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
};

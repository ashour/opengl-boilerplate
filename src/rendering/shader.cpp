#include "lib/file_to_string.h"
#include "lib/log.h"
#include "rendering/rendering.h"
#include "shader.h"
#include <string>
#include <unordered_map>

namespace eo
{
Shader::Shader(const std::string& vertex_shader_filepath,
               const std::string& fragment_shader_filepath)
    : _vertex_shader_filepath{vertex_shader_filepath},
      _fragment_shader_filepath{fragment_shader_filepath},
      _shader_program{0},
      _uniform_locations{}
{
}

Shader::~Shader()
{
    gldc(glDeleteProgram(_shader_program));
    _shader_program = 0;
}

void Shader::build()
{
    constexpr size_t log_buffer_size = 512;
    char log_buffer[log_buffer_size];

    unsigned int vertex_shader =
        create_shader(GL_VERTEX_SHADER, _vertex_shader_filepath, log_buffer, log_buffer_size);

    unsigned int fragment_shader =
        create_shader(GL_FRAGMENT_SHADER, _fragment_shader_filepath, log_buffer, log_buffer_size);

    _shader_program = create_program(vertex_shader, fragment_shader, log_buffer, log_buffer_size);

    delete_shader(vertex_shader);
    delete_shader(fragment_shader);
}

void Shader::use() const { gldc(glUseProgram(_shader_program)); }

void Shader::unuse_all() { gldc(glUseProgram(0)); }

int Shader::uniform_location_for(const std::string& variable)
{
    auto it = _uniform_locations.find(variable);
    if (it == _uniform_locations.end())
    {
        gldc(int location = glGetUniformLocation(_shader_program, variable.c_str()));
        EO_ASSERT(location > -1);
        _uniform_locations[variable] = location;
        return location;
    }
    return it->second;
}

unsigned int Shader::create_shader(const GLenum type,
                                   const std::string& filepath,
                                   char* log_buffer,
                                   const size_t log_buffer_size) const
{
    gldc(unsigned int shader = glCreateShader(type));

    std::string source{file_to_string(filepath)};
    const char* source_cstr = source.c_str();
    gldc(glShaderSource(shader, 1, &source_cstr, nullptr));

    gldc(glCompileShader(shader));

    int success;
    gldc(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        gldc(glGetShaderInfoLog(shader, log_buffer_size, nullptr, log_buffer));
        EO_LOG_ERROR("Failed to compile shader: {}", log_buffer);
    }

    return shader;
}

void Shader::delete_shader(const unsigned int shader) const { gldc(glDeleteShader(shader)); }

unsigned int Shader::create_program(const unsigned int vertex_shader,
                                    const unsigned int fragment_shader,
                                    char* log_buffer,
                                    const size_t log_buffer_size) const
{
    unsigned int program;
    gldc(program = glCreateProgram());
    gldc(glAttachShader(program, vertex_shader));
    gldc(glAttachShader(program, fragment_shader));
    gldc(glLinkProgram(program));

    int success;
    gldc(glGetProgramiv(program, GL_LINK_STATUS, &success));
    if (!success)
    {
        gldc(glGetShaderInfoLog(program, log_buffer_size, nullptr, log_buffer));
        EO_LOG_ERROR("Failed to attach or link shaders to program: {}", log_buffer);
    }

    return program;
}
} // namespace eo

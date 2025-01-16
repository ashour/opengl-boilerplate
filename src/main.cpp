// clang-format off
#include <glad/glad.h>
// clang-format on
#include "opengl_debug.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>

static constexpr unsigned int OPENGL_MAJOR_VERSION = 4;
static constexpr unsigned int OPENGL_MINOR_VERSION = 1;

static const char *WINDOW_TITLE = "OpenGL Boilerplate";
static constexpr unsigned int WINDOW_WIDTH = 480;
static constexpr unsigned int WINDOW_HEIGHT = 480;

const char *vertex_shader_source = R"(
#version 410 core

layout (location = 0) in vec3 i_position;

void main()
{
  gl_Position = vec4(i_position.x, i_position.y, i_position.z, 1.0);
}
)";

const char *fragment_shader_source = R"(
#version 410 core

out vec4 o_color;

void main()
{
  o_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

int main()
{
    std::cout << std::endl << "=== MAIN START ===" << std::endl;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    std::cout << "Apple env detected" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    std::cout << std::endl << "=== INITIALIZATION COMPLETE ===" << std::endl;
    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

    int buffer_width;
    int buffer_height;
    glfwGetFramebufferSize(window, &buffer_width, &buffer_height);
    gldc(glViewport(0, 0, buffer_width, buffer_height));

    // clang-format off
    float vertices[] = {
        -0.5f, -0.413675f, 0.0f,
         0.5f, -0.413675f, 0.0f,
         0.0f,  0.45235f, 0.0f
    };
    // clang-format on

    unsigned int VAO;
    gldc(glGenVertexArrays(1, &VAO));
    gldc(glBindVertexArray(VAO));

    unsigned int VBO;
    gldc(glGenBuffers(1, &VBO));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    gldc(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    gldc(unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER));
    gldc(glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr));
    gldc(glCompileShader(vertex_shader));

    int success;
    char info_log[512];
    gldc(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        gldc(glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log));
        std::cerr << "Failed to compile vertex shader: " << info_log << std::endl;
    }

    gldc(unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER));
    gldc(glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr));
    gldc(glCompileShader(fragment_shader));

    gldc(glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        gldc(glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log));
        std::cerr << "Failed to compile fragment shader: " << info_log << std::endl;
    }

    gldc(unsigned int shader_program = glCreateProgram());
    gldc(glAttachShader(shader_program, vertex_shader));
    gldc(glAttachShader(shader_program, fragment_shader));
    gldc(glLinkProgram(shader_program));

    gldc(glGetProgramiv(shader_program, GL_LINK_STATUS, &success));
    if (!success)
    {
        gldc(glGetShaderInfoLog(shader_program, 512, nullptr, info_log));
        std::cerr << "Failed to attach or link shaders to program: " << info_log << std::endl;
    }

    gldc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));
    gldc(glEnableVertexAttribArray(0));

    while (!glfwWindowShouldClose(window))
    {
        gldc(glClear(GL_COLOR_BUFFER_BIT));

        gldc(glUseProgram(shader_program));
        gldc(glBindVertexArray(VAO));
        gldc(glDrawArrays(GL_TRIANGLES, 0, 3));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

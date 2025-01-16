// clang-format off
#include <glad/glad.h>
// clang-format on
#include "lib/color.h"
#include "lib/file_to_string.h"
#include "lib/opengl_debug.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>

static constexpr unsigned int OPENGL_MAJOR_VERSION = 4;
static constexpr unsigned int OPENGL_MINOR_VERSION = 1;

static constexpr std::string WINDOW_TITLE = "OpenGL Boilerplate";
static constexpr unsigned int WINDOW_WIDTH = 480;
static constexpr unsigned int WINDOW_HEIGHT = 480;

static constexpr const char* VERTEX_SHADER_FILEPATH = "./resources/shaders/default.vert";
static constexpr const char* FRAGMENT_SHADER_FILEPATH = "./resources/shaders/default.frag";

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

    GLFWwindow* window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), nullptr, nullptr);
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
        -0.5f, -0.5f, 0.0f, NCOLV(239.0),  NCOLV(71.0), NCOLV(111.0),
         0.5f, -0.5f, 0.0f, NCOLV(255.0), NCOLV(181.0),  NCOLV(99.0),
         0.5f,  0.5f, 0.0f, NCOLV(176.0), NCOLV(219.0),  NCOLV(67.0),
        -0.5f,  0.5f, 0.0f,  NCOLV(84.0), NCOLV(242.0), NCOLV(242.0),
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    // clang-format on

    unsigned int VAO;
    gldc(glGenVertexArrays(1, &VAO));
    gldc(glBindVertexArray(VAO));

    unsigned int VBO;
    gldc(glGenBuffers(1, &VBO));
    gldc(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    gldc(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    unsigned int EBO;
    gldc(glGenBuffers(1, &EBO));
    gldc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    gldc(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    gldc(unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER));

    std::string vertex_shader_source = file_to_string(VERTEX_SHADER_FILEPATH);
    const char* vertex_shader_source_cstr = vertex_shader_source.c_str();
    gldc(glShaderSource(vertex_shader, 1, &vertex_shader_source_cstr, nullptr));
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
    std::string fragment_shader_source = file_to_string(FRAGMENT_SHADER_FILEPATH);
    const char* fragment_shader_source_cstr = fragment_shader_source.c_str();
    gldc(glShaderSource(fragment_shader, 1, &fragment_shader_source_cstr, nullptr));
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

    gldc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    gldc(glEnableVertexAttribArray(0));
    gldc(glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
    gldc(glEnableVertexAttribArray(1));

    while (!glfwWindowShouldClose(window))
    {
        gldc(glClearColor(NCOLV(36.0), NCOLV(22.0), NCOLV(35), 1.0));
        gldc(glClear(GL_COLOR_BUFFER_BIT));

        gldc(glUseProgram(shader_program));
        gldc(glBindVertexArray(VAO));
        gldc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
        gldc(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// clang-format off
#include <glad/glad.h>
// clang-format on
#include "glm/trigonometric.hpp"
#include "lib/color.h"
#include "lib/log.h"
#include "lib/opengl_debug.h"
#include "shader.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

static constexpr unsigned int OPENGL_MAJOR_VERSION = 4;
static constexpr unsigned int OPENGL_MINOR_VERSION = 1;

static constexpr std::string WINDOW_TITLE = "OpenGL Boilerplate";
static constexpr unsigned int WINDOW_WIDTH = 800;
static constexpr unsigned int WINDOW_HEIGHT = 600;

static constexpr const char* VERTEX_SHADER_FILEPATH = "./resources/shaders/default.vert";
static constexpr const char* FRAGMENT_SHADER_FILEPATH = "./resources/shaders/default.frag";

int main()
{
    LOG_H("Main Start");

    std::unique_ptr<Window> window;

    try
    {
        window = std::unique_ptr<Window>{new Window{
            WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION}};
    }
    catch (const WindowException& e)
    {
        LOG_ERR(e.what());
        return -1;
    }

    LOG_H("Initialization Complete");
    LOG("OpenGL version " << window->opengl_version());

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

    Shader shader(VERTEX_SHADER_FILEPATH, FRAGMENT_SHADER_FILEPATH);
    shader.build();

    shader.use();

    gldc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    gldc(glEnableVertexAttribArray(0));
    gldc(glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
    gldc(glEnableVertexAttribArray(1));

    window->set_clear_color(glm::vec4{NCOLV(36.0), NCOLV(22.0), NCOLV(35), 1.0});

    shader.use();
    unsigned int u_model = shader.uniform_location_for("u_model");
    unsigned int u_view = shader.uniform_location_for("u_view");
    unsigned int u_projection = shader.uniform_location_for("u_projection");
    Shader::unuse_all();

    while (!window->should_close())
    {
        window->clear();

        shader.use();

        glm::mat4 model{1.0f};
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shader.set_uniform_mat4(u_model, model);

        glm::mat4 view{1.0f};
        view = glm::translate(view, glm::vec3{0.0f, -0.25f, -3.0f});
        shader.set_uniform_mat4(u_view, view);

        glm::mat4 projection{
            glm::perspective(glm::radians(45.0f),
                             (float)window->buffer_width() / (float)window->buffer_height(),
                             0.1f,
                             100.0f)};
        shader.set_uniform_mat4(u_projection, projection);

        gldc(glBindVertexArray(VAO));
        gldc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
        gldc(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        window->swap_buffers();
        window->poll_events();
    }

    return 0;
}

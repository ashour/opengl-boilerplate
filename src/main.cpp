// clang-format off
#include <glad/glad.h>
// clang-format on
#include "lib/color.h"
#include "lib/log.h"
#include "lib/opengl_debug.h"
#include "rendering/camera.h"
#include "rendering/shader.h"
#include "system/input.h"
#include "system/window.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

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
        // Front face
        -0.5f, -0.5f,  0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 0
         0.5f, -0.5f,  0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 1
         0.5f,  0.5f,  0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 2
        -0.5f,  0.5f,  0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 3
        // Back face
        -0.5f, -0.5f, -0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 4
         0.5f, -0.5f, -0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 5
         0.5f,  0.5f, -0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 6
        -0.5f,  0.5f, -0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX  // 7
    };

    unsigned int indices[] = {
        // Front
        0, 1, 2,
        2, 3, 0,
        // Right
        1, 5, 6,
        6, 2, 1,
        // Back
        7, 6, 5,
        5, 4, 7,
        // Left
        4, 0, 3,
        3, 7, 4,
        // Top
        3, 2, 6,
        6, 7, 3,
        // Bottom
        4, 5, 1,
        1, 0, 4
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

    Camera camera{
        static_cast<float>(window->buffer_width() / static_cast<float>(window->buffer_height()))};
    shader.set_uniform_mat4(u_projection, camera.projection());
    Shader::unuse_all();

    Input::register_mouse_move_handler(
        [&camera](glm::vec2 current_mouse_position, glm::vec2 last_mouse_position) -> void
        { camera.look(current_mouse_position, last_mouse_position); });

    float last_frame_time = 0.0f;
    bool is_running = true;
    while (is_running)
    {
        float current_frame_time = glfwGetTime();
        float delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        window->poll_events();
        window->clear();

        if (Input::action_pressed(Action::move_forward))
        {
            camera.strafe(Strafe::forward, delta_time);
        }
        if (Input::action_pressed(Action::move_back))
        {
            camera.strafe(Strafe::back, delta_time);
        }
        if (Input::action_pressed(Action::move_left))
        {
            camera.strafe(Strafe::left, delta_time);
        }
        if (Input::action_pressed(Action::move_right))
        {
            camera.strafe(Strafe::right, delta_time);
        }

        shader.use();
        shader.set_uniform_mat4(u_view, camera.view());

        glm::mat4 model{1.0f};
        model = glm::rotate(
            model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shader.set_uniform_mat4(u_model, model);

        gldc(glBindVertexArray(VAO));
        gldc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
        gldc(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));

        model = {1.0f};
        model = glm::translate(model, glm::vec3{2.0f, 0.0, 0.0});
        model = glm::rotate(
            model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shader.set_uniform_mat4(u_model, model);

        gldc(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));

        window->swap_buffers();

        is_running = !(window->should_close() || Input::action_pressed(Action::quit_app));
    }

    return 0;
}

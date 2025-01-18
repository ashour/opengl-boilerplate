// clang-format off
#include <glad/glad.h>
// clang-format on
#include "lib/color.h"
#include "lib/log.h"
#include "objects/cube.h"
#include "objects/plane.h"
#include "rendering/camera.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "rendering/transform.h"
#include "system/input.h"
#include "system/time.h"
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

void render_scene(eo::Shader& shader)
{
    eo::Mesh cube{eo::cube};

    eo::Transform cube_transform{};
    cube_transform.position({0.0f, 7.5f, -5.0f});
    cube_transform.rotation(static_cast<float>(glfwGetTime()) * glm::radians(50.0f),
                            {0.5f, 1.0f, 0.0f});
    unsigned int u_model = shader.uniform_location_for("u_model");
    shader.set_uniform_mat4(u_model, cube_transform.matrix());

    cube.draw();

    cube_transform.position({2.0f, 7.5f, -5.0f});
    shader.set_uniform_mat4(u_model, cube_transform.matrix());

    cube.draw();

    eo::Mesh plane{eo::plane};

    shader.set_uniform_mat4(u_model, glm::mat4{1.0f});

    plane.draw();
}

int main()
{
    LOG_H("Main Start");

    std::unique_ptr<eo::Window> window;

    try
    {
        window = std::unique_ptr<eo::Window>{new eo::Window{
            WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION}};
    }
    catch (const eo::WindowException& e)
    {
        LOG_ERR(e.what());
        return -1;
    }

    LOG_H("Initialization Complete");
    LOG("OpenGL version " << window->opengl_version());

    eo::Shader shader(VERTEX_SHADER_FILEPATH, FRAGMENT_SHADER_FILEPATH);
    shader.build();

    window->set_clear_color(glm::vec4{NCOLV(36.0), NCOLV(22.0), NCOLV(35), 1.0});

    shader.use();
    unsigned int u_view = shader.uniform_location_for("u_view");
    unsigned int u_projection = shader.uniform_location_for("u_projection");

    eo::Camera camera{
        static_cast<float>(window->buffer_width() / static_cast<float>(window->buffer_height()))};
    shader.set_uniform_mat4(u_projection, camera.projection());
    eo::Shader::unuse_all();

    eo::Input::register_mouse_move_handler(
        [&camera](glm::vec2 current_mouse_position, glm::vec2 last_mouse_position) -> void
        { camera.look(current_mouse_position, last_mouse_position); });

    bool is_running = true;
    while (is_running)
    {
        eo::Time::update();

        window->poll_events();
        window->clear();

        if (eo::Input::action_pressed(eo::Action::move_forward))
        {
            camera.strafe(eo::Strafe::forward, eo::Time::delta_time());
        }
        if (eo::Input::action_pressed(eo::Action::move_back))
        {
            camera.strafe(eo::Strafe::back, eo::Time::delta_time());
        }
        if (eo::Input::action_pressed(eo::Action::move_left))
        {
            camera.strafe(eo::Strafe::left, eo::Time::delta_time());
        }
        if (eo::Input::action_pressed(eo::Action::move_right))
        {
            camera.strafe(eo::Strafe::right, eo::Time::delta_time());
        }

        shader.use();
        shader.set_uniform_mat4(u_view, camera.view());

        render_scene(shader);

        window->swap_buffers();

        is_running = !(window->should_close() || eo::Input::action_pressed(eo::Action::quit_app));
    }

    return 0;
}

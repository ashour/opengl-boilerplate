#include "input.h"
#include "lib/log.h"
#include "rendering/rendering.h"
#include "window.h"

namespace eo
{
Window::Window(const unsigned int width,
               const unsigned int height,
               const std::string& title,
               const unsigned int opengl_major_version,
               const unsigned int opengl_minor_version)
{
    if (!glfwInit())
    {
        throw WindowException("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_minor_version);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    EO_LOG_INFO("Apple environment detected");
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    _glfw_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!_glfw_window)
    {
        throw WindowException("Could not create GLFW window");
    }

    glfwSetKeyCallback(_glfw_window, Input::key_callback);
    glfwSetCursorPosCallback(_glfw_window, Input::mouse_callback);
    glfwSetMouseButtonCallback(_glfw_window, Input::mouse_button_callback);

    glfwMakeContextCurrent(_glfw_window);

    glfwGetFramebufferSize(_glfw_window, &_buffer_width, &_buffer_height);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw WindowException{"Failed to initialize GLAD"};
    }

    gldc(glEnable(GL_DEPTH_TEST));
    gldc(glEnable(GL_CULL_FACE));

    gldc(glViewport(0, 0, _buffer_width, _buffer_height));
}

Window::~Window()
{
    glfwDestroyWindow(_glfw_window);
    glfwTerminate();
    _glfw_window = nullptr;
}

bool Window::should_close() const { return glfwWindowShouldClose(_glfw_window); }

void Window::set_clear_color(const glm::vec4& color) const
{
    gldc(glClearColor(color.r, color.g, color.b, color.a));
}

void Window::clear() const { gldc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); }

void Window::swap_buffers() const { glfwSwapBuffers(_glfw_window); }

void Window::poll_events() const { glfwPollEvents(); }

void Window::enable_vsync(bool enable) const { glfwSwapInterval(enable); }

const unsigned char* Window::opengl_version() const { return glGetString(GL_VERSION); }
} // namespace eo

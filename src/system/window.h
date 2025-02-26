#pragma once

#include "rendering/rendering.h"
#include <string>

namespace eo
{
class Window
{
  public:
    Window(const unsigned int width,
           const unsigned int height,
           const std::string& title,
           const unsigned int opengl_major_version,
           const unsigned int opengl_minor_version);
    ~Window();

    const int buffer_width() const { return _buffer_width; }
    const int buffer_height() const { return _buffer_height; }

    GLFWwindow* native_window() const { return _glfw_window; }

    bool should_close() const;
    void set_clear_color(const glm::vec4& color) const;
    void clear() const;
    void swap_buffers() const;
    void poll_events() const;

    void enable_vsync(bool enable) const;

    const unsigned char* opengl_version() const;

  private:
    GLFWwindow* _glfw_window;
    int _buffer_width;
    int _buffer_height;
};

class WindowException : public std::runtime_error
{
  public:
    explicit WindowException(const char* message) : std::runtime_error(message) {}
};
} // namespace eo

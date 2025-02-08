#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>

namespace eo
{
class UI
{
  public:
    UI(GLFWwindow* window);
    ~UI();

    void new_frame();
    void render();

    bool begin_window(const std::string& name);
    void end_window();

    bool radio_button(const std::string& label, bool is_selected);
};
} // namespace eo

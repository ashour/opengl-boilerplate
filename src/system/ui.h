#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace eo
{
class UI
{
  public:
    UI(GLFWwindow* window);
    ~UI();

    void new_frame();
    void render();
};
} // namespace eo

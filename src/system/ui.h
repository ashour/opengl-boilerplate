#pragma once

#include "rendering/rendering.h"
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

    void show_demo();
    void show_metrics(const unsigned int rendered_fragment_count,
                      const unsigned int rendered_vertex_count);

    bool begin_window(const std::string& name);
    void end_window();

    void push_item_full_width();
    void pop_item_full_width();

    void text(const std::string& label);
    bool radio_button(const std::string& label, bool is_selected);
    void drag_float(const std::string& label, float* value);
};
} // namespace eo

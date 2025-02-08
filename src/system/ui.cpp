#include "system/time.h"
#include "ui.h"

namespace eo
{

UI::UI(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

UI::~UI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::new_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::show_demo() { ImGui::ShowDemoWindow(); }

void UI::show_metrics()
{
    begin_window("Metrics");
    auto delta_time = Time::delta_time();
    float fps = (delta_time > 0.0f) ? (1.0f / delta_time) : 0.0f;
    ImGui::Text("%.3f ms/frame (%.1f FPS)", delta_time * 1000, fps);
    end_window();
}

bool UI::begin_window(const std::string& name) { return ImGui::Begin(name.c_str()); }

void UI::end_window() { ImGui::End(); }

bool UI::radio_button(const std::string& label, bool is_selected)
{
    return ImGui::RadioButton(label.c_str(), is_selected);
}

} // namespace eo

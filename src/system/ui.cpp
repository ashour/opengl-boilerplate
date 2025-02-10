#include "system/metrics.h"
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
    ImGui::Text(
        "%.3f ms/frame (%.1f FPS)", Time::delta_time() * 1000, Metrics::frames_per_second());
    end_window();
}

bool UI::begin_window(const std::string& name) { return ImGui::Begin(name.c_str()); }

void UI::end_window() { ImGui::End(); }

bool UI::radio_button(const std::string& label, bool is_selected)
{
    return ImGui::RadioButton(label.c_str(), is_selected);
}

} // namespace eo

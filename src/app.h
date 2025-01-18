#include "rendering/camera.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <memory>
#include <string>

namespace eo
{

static constexpr unsigned int OPENGL_MAJOR_VERSION = 4;
static constexpr unsigned int OPENGL_MINOR_VERSION = 1;

static constexpr std::string WINDOW_TITLE = "OpenGL Boilerplate";
static constexpr unsigned int WINDOW_WIDTH = 800;
static constexpr unsigned int WINDOW_HEIGHT = 600;

static constexpr const char* VERTEX_SHADER_FILEPATH = "./resources/shaders/default.vert";
static constexpr const char* FRAGMENT_SHADER_FILEPATH = "./resources/shaders/default.frag";

class App
{
  public:
    int run();

  private:
    bool init_window();
    void init_rendering();
    void init_input();

    void loop();
    void handle_input();
    void render_scene();

    std::unique_ptr<Window> _window;
    std::unique_ptr<Camera> _camera;
    std::unique_ptr<Shader> _shader;

    unsigned int _u_view;
};
} // namespace eo

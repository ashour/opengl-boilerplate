#include "rendering/camera.h"
#include "rendering/shader.h"
#include "system/window.h"
#include <array>
#include <memory>

namespace eo
{

class App
{
  public:
    int run();

  private:
    bool init_window();
    void init_rendering();
    void init_texture();
    void init_input();
    void init_cube_positions();

    void loop();
    void handle_input();
    void render_scene();

    std::unique_ptr<Window> _window;
    std::unique_ptr<Camera> _camera;
    std::unique_ptr<Shader> _shader;

    unsigned int _u_view;

    std::array<glm::vec3, 180> _cube_positions{};

    unsigned int _texture;
};
} // namespace eo

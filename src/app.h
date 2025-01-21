#include "rendering/camera.h"
#include "rendering/shader.h"
#include "rendering/texture.h"
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
    void init_input();
    void init_cube_positions();

    void loop();
    void handle_input();
    void render_scene();

    std::unique_ptr<Window> _window;
    std::unique_ptr<Camera> _camera;
    std::unique_ptr<Shader> _shader;

    std::unique_ptr<Texture> _texture;

    unsigned int _u_view;

    std::array<glm::vec3, 180> _cube_positions{};
};
} // namespace eo

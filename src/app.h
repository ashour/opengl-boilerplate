#include "rendering/camera.h"
#include "rendering/lights/directional_light.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
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
    void init_input();
    void init_cube_positions();

    void loop();
    void handle_input();
    void render_scene();

    std::unique_ptr<Window> _window;
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<DirectionalLight> _light;

    std::unique_ptr<Material> _mat_dirt;
    std::unique_ptr<Material> _mat_box;

    int _u_view_matrix;
    int _u_view_position;

    std::unique_ptr<Mesh> _cube;
    std::unique_ptr<Mesh> _plane;
    std::array<glm::vec3, 180> _cube_positions{};
};
} // namespace eo

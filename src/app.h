#include "rendering/camera.h"
#include "rendering/lights/directional_light.h"
#include "rendering/lights/spot_light.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/model.h"
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
    std::unique_ptr<DirectionalLight> _directional_light;
    std::unique_ptr<SpotLight> _flash_light;

    std::shared_ptr<Material> _mat_dirt;
    std::shared_ptr<Material> _mat_box;

    int _u_view_matrix;
    int _u_view_position;

    std::unique_ptr<Mesh> _cube;
    std::array<glm::vec3, 180> _cube_positions{};

    std::unique_ptr<Mesh> _plane;
    std::unique_ptr<Model> _x_wing;
    std::unique_ptr<Model> _backpack;
};
} // namespace eo

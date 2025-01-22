#include "rendering/camera.h"
#include "rendering/light.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
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
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<Light> _light;

    std::unique_ptr<Texture> _wall_texture;
    std::unique_ptr<Texture> _dirt_texture;

    std::unique_ptr<Material> _mat_green_clay;
    std::unique_ptr<Material> _mat_gold;

    unsigned int _u_view_matrix;
    unsigned int _u_view_position;

    std::unique_ptr<Mesh> _cube;
    std::unique_ptr<Mesh> _plane;
    std::array<glm::vec3, 180> _cube_positions{};
};
} // namespace eo

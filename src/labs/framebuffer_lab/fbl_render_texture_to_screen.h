#include "labs/lab.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/shader.h"
#include <memory>
namespace eo
{
class Fbl_RenderTextureToScreen : public Lab
{
  public:
    Fbl_RenderTextureToScreen(const Window& window,
                              const std::string& screen_fragment_shader_file_path,
                              const bool upload_screen_dimensions = false,
                              const std::vector<float>* kernel = nullptr);
    ~Fbl_RenderTextureToScreen();

    void on_update() override;
    void on_render() override;

  private:
    std::unique_ptr<Camera> _camera;
    std::shared_ptr<Shader> _scene_shader;
    std::shared_ptr<Shader> _quad_shader;

    std::shared_ptr<Material> _mat_dirt;
    std::shared_ptr<Material> _mat_box;

    std::unique_ptr<Mesh> _quad;

    std::unique_ptr<Mesh> _cube;
    std::array<glm::vec3, 180> _cube_positions{};

    std::unique_ptr<Mesh> _ground;

    unsigned int _fbo;
    unsigned int _rbo;
    unsigned int _tex_color_buffer;

    void render_scene();
};
} // namespace eo

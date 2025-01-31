#pragma once

#include "rendering/mesh.h"
#include "rendering/shader.h"
#include <assimp/scene.h>
#include <memory>
#include <string>
#include <vector>

namespace eo
{
class Model
{
  public:
    Model(const std::string& file_path);

    void draw(Shader& shader);

  private:
    std::vector<std::unique_ptr<Mesh>> _meshes;
    std::string _directory;

    void load_model(std::string file_path);

    void process_node(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> process_mesh(aiMesh* mesh, const aiScene* scene);

    std::vector<NewTexture>
    load_textures_for(aiMaterial* material, aiTextureType type, std::string type_name);
    static unsigned int tex_from_file(const char* path, const std::string& directory);

    std::vector<NewTexture> _loaded_texture_cache;
};
} // namespace eo

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

    void draw(Shader& shader, bool use_materials = true);
    void unbind_materials(Shader& shader);

  private:
    std::vector<std::unique_ptr<Mesh>> _meshes;
    std::string _directory;

    void load_model(std::string file_path);

    void process_node(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> process_mesh(aiMesh* mesh, const aiScene* scene);

    std::vector<std::shared_ptr<Texture>>
    load_textures_for(aiMaterial* material, aiTextureType ai_type, Texture::Type internal_type);

    std::string actual_file_path_for(const char* model_texture_file_path);

    std::vector<std::shared_ptr<Texture>> _loaded_texture_cache;
};
} // namespace eo

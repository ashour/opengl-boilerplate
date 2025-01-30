#pragma once

#include "rendering/camera.h"
#include "rendering/shader.h"
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace eo
{
struct AssimpVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct AssimpTexture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class AssimpMesh
{
  public:
    std::vector<AssimpVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<AssimpTexture> textures;

    AssimpMesh(std::vector<AssimpVertex> vertices,
               std::vector<unsigned int> inidices,
               std::vector<AssimpTexture> textures);

    void draw(Shader& shader) const;

  private:
    unsigned int _vao;
    unsigned int _vbo;
    unsigned int _ebo;

    void setup_mesh();
};

class AssimpModel
{
  public:
    AssimpModel(const std::string& file_path);

    void draw(Shader& shader);

  private:
    std::vector<AssimpMesh> _meshes;
    std::string _directory;

    void load_model(std::string file_path);
    void process_node(aiNode* node, const aiScene* scene);
    AssimpMesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<AssimpTexture>
    load_textures_for(aiMaterial* material, aiTextureType type, std::string type_name);

    static unsigned int tex_from_file(const char* path, const std::string& directory);
};

class AssimpTest
{
  public:
    int run();

  private:
    void handle_input();

    std::unique_ptr<Camera> _camera;
};
} // namespace eo

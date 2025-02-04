// clang-format off
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// clang-format on

#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace eo
{

Model::Model(const std::string& file_path) { load_model(file_path); }

void Model::draw(Shader& shader)
{
    for (const auto& mesh : _meshes)
    {
        mesh->draw(shader);
    }
}

void Model::unbind_materials(Shader& shader)
{

    for (const auto& mesh : _meshes)
    {
        mesh->material()->unbind(shader);
    }
}

void Model::load_model(std::string file_path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        file_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        EO_LOG_ERROR("[Assimp] " << importer.GetErrorString());
        return;
    }

    _directory = file_path.substr(0, file_path.find_last_not_of('/'));

    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
    _meshes.reserve(_meshes.size() + node->mNumMeshes);

    for (size_t i = 0; i < node->mNumMeshes; i += 1)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(process_mesh(mesh, scene));
    }

    for (size_t i = 0; i < node->mNumChildren; i += 1)
    {
        process_node(node->mChildren[i], scene);
    }
}

std::unique_ptr<Mesh> Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    vertices.reserve(mesh->mNumVertices);

    for (size_t i = 0; i < mesh->mNumVertices; i += 1)
    {
        Vertex vertex;
        vertex.position =
            glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        if (mesh->mTextureCoords[0])
        {
            vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.uv = glm::vec2(0.0f);
        }

        vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i += 1)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j += 1)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    float shininess;

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        auto diffuse_maps = load_textures_for(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        auto specular_maps = load_textures_for(material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

        if (material->Get(AI_MATKEY_SHININESS, shininess) != AI_SUCCESS)
        {
            shininess = 0;
        }
    }

    auto material = std::make_shared<Material>(textures, shininess);
    return std::make_unique<Mesh>(std::move(vertices), std::move(indices), material);
}

std::vector<std::shared_ptr<Texture>>
Model::load_textures_for(aiMaterial* material, aiTextureType type, std::string type_name)
{
    std::vector<std::shared_ptr<Texture>> textures;

    auto texture_count = material->GetTextureCount(type);
    if (texture_count == 0 && type_name == "specular")
    {
        textures.push_back(Texture::no_specular());
        return textures;
    }

    for (size_t i = 0; i < texture_count; i += 1)
    {
        aiString model_file_path;
        material->GetTexture(type, i, &model_file_path);
        std::string actual_filepath = actual_file_path_for(model_file_path.C_Str());

        bool was_loaded_from_cache = false;
        for (size_t j = 0; j < _loaded_texture_cache.size(); j += 1)
        {
            if (_loaded_texture_cache[j]->path() == actual_filepath)
            {
                textures.push_back(_loaded_texture_cache[j]);
                was_loaded_from_cache = true;
                break;
            }
        }

        if (!was_loaded_from_cache)
        {
            auto texture = std::make_shared<Texture>(type_name, actual_filepath);
            textures.push_back(texture);
            _loaded_texture_cache.push_back(texture);
        }
    }

    return textures;
}

std::string Model::actual_file_path_for(const char* model_texture_file_path)
{
    std::string model_file_path = std::string(model_texture_file_path);
    std::string filename = model_file_path.substr(model_file_path.find_last_of("/\\") + 1);
    return _directory + '/' + filename;
}

} // namespace eo

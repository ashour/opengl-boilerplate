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

void Model::load_model(std::string file_path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);
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
    std::vector<NewTexture> textures;

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

        std::vector<NewTexture> diffuse_maps =
            load_textures_for(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        std::vector<NewTexture> specular_maps =
            load_textures_for(material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

        if (material->Get(AI_MATKEY_SHININESS, shininess) != AI_SUCCESS)
        {
            shininess = 0;
        }
    }

    return std::make_unique<Mesh>(
        std::move(vertices), std::move(indices), std::move(textures), shininess);
}

std::vector<NewTexture>
Model::load_textures_for(aiMaterial* material, aiTextureType type, std::string type_name)
{
    std::vector<NewTexture> textures;

    for (size_t i = 0; i < material->GetTextureCount(type); i += 1)
    {
        aiString model_file_path;
        material->GetTexture(type, i, &model_file_path);
        std::string actual_filepath = actual_file_path_for(model_file_path.C_Str());

        bool was_loaded_from_cache = false;
        for (size_t j = 0; j < _loaded_texture_cache.size(); j += 1)
        {

            if (_loaded_texture_cache[j].path == actual_filepath)
            {
                textures.push_back(_loaded_texture_cache[j]);
                was_loaded_from_cache = true;
                break;
            }
        }

        if (!was_loaded_from_cache)
        {
            NewTexture texture;
            texture.id = tex_from_file(actual_filepath.c_str());
            texture.type = type_name;
            texture.path = actual_filepath;
            textures.push_back(texture);
            _loaded_texture_cache.push_back(texture);
        }
    }

    return textures;
}

unsigned int Model::tex_from_file(const char* file_path)
{
    // TODO should really be the Texture object's responsbility
    unsigned int texture_id;
    gldc(glGenTextures(1, &texture_id));

    int width, height, component_count;
    unsigned char* data = stbi_load(file_path, &width, &height, &component_count, 0);
    if (data)
    {
        GLenum format;
        if (component_count == 1)
        {
            format = GL_RED;
        }
        else if (component_count == 3)
        {
            format = GL_RGB;
        }
        else if (component_count == 4)
        {
            format = GL_RGBA;
        }

        gldc(glBindTexture(GL_TEXTURE_2D, texture_id));
        gldc(glTexImage2D(
            GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        gldc(glGenerateMipmap(GL_TEXTURE_2D));

        gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << file_path << std::endl;
        stbi_image_free(data);
    }

    return texture_id;
}

std::string Model::actual_file_path_for(const char* model_texture_file_path)
{
    std::string model_file_path = std::string(model_texture_file_path);
    std::string filename = model_file_path.substr(model_file_path.find_last_of("/\\") + 1);
    return _directory + '/' + filename;
}

} // namespace eo

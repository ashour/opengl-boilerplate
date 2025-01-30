// clang-format off
#include "rendering/camera.h"
#include "rendering/transform.h"
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// clang-format on
#include "assimp_test.h"
#include "config.h"
#include "lib/log.h"
#include "lib/opengl_debug.h"
#include "system/input.h"
#include "system/time.h"
#include "system/window.h"
#include <assimp/DefaultLogger.hpp>
#include <assimp/Importer.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <string>

namespace eo
{

class ConsoleLogger : public Assimp::LogStream
{
  public:
    void write(const char* message) override { EO_LOG_INFO(message); }
};

int AssimpTest::run()
{
    EO_LOG_INFO("AssimpTest");
    Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
    Assimp::Logger* logger = Assimp::DefaultLogger::get();
    logger->attachStream(new ConsoleLogger(), Assimp::Logger::VERBOSE | Assimp::Logger::DEBUGGING);
    stbi_set_flip_vertically_on_load(true);

    std::unique_ptr<Window> window;
    try
    {
        window = std::make_unique<Window>(
            WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
    }
    catch (const WindowException& e)
    {
        EO_LOG_ERROR(e.what());
        return -1;
    }
    window->set_clear_color(SCENE_CLEAR_COLOR);

    Shader shader{SHADER_DIR + "assimp_test.vert", SHADER_DIR + "assimp_test.frag"};
    shader.build();
    shader.use();

    _camera = std::make_unique<Camera>(static_cast<float>(window->buffer_width()) /
                                       static_cast<float>(window->buffer_height()));

    shader.set_uniform("u_projection", _camera->projection());
    shader.set_uniform("u_view", _camera->view());

    Input::register_mouse_move_handler(
        [this](auto current_mouse_position, auto last_mouse_position)
        { _camera->look(current_mouse_position, last_mouse_position); });

    AssimpModel assimp_model{"resources/models/x-wing.obj"};

    Transform transform;
    transform.position(glm::vec3(-140.0f, -10.0f, 100.0f));
    transform.scale(glm::vec3(0.1f));
    bool is_running = true;
    while (is_running)
    {
        Time::update();

        window->poll_events();
        window->clear();

        handle_input();

        shader.set_uniform("u_projection", _camera->projection());
        shader.set_uniform("u_view", _camera->view());

        shader.set_uniform("u_model", transform.matrix());
        assimp_model.draw(shader);

        window->swap_buffers();

        is_running = !(window->should_close() || Input::action_pressed(Action::quit_app));
    }

    Assimp::DefaultLogger::kill();
    return 0;
}

void AssimpTest::handle_input()
{
    if (Input::action_pressed(Action::move_forward))
    {
        _camera->strafe(Strafe::forward, Time::delta_time());
    }
    if (Input::action_pressed(Action::move_back))
    {
        _camera->strafe(Strafe::back, Time::delta_time());
    }
    if (Input::action_pressed(Action::move_left))
    {
        _camera->strafe(Strafe::left, Time::delta_time());
    }
    if (Input::action_pressed(Action::move_right))
    {
        _camera->strafe(Strafe::right, Time::delta_time());
    }
}

AssimpMesh::AssimpMesh(std::vector<AssimpVertex> vertices,
                       std::vector<unsigned int> inidices,
                       std::vector<AssimpTexture> textures)
    : vertices{vertices}, indices{inidices}, textures{textures}
{
    setup_mesh();
}

void AssimpMesh::draw(Shader& shader) const
{
    unsigned int diffuse_counter = 1;
    unsigned int specular_counter = 1;

    for (int i = 0; i < textures.size(); i += 1)
    {
        gldc(glActiveTexture(GL_TEXTURE0 + i));

        std::string number;
        std::string name = textures[i].type;

        if (name == "diffuse")
        {
            number = std::to_string(diffuse_counter);
            diffuse_counter += 1;
        }
        else if (name == "specular")
        {
            number = std::to_string(specular_counter);
            specular_counter += 1;
        }

        shader.set_uniform(std::format("tex_{}_{}", name, number), i);
        gldc(glBindTexture(GL_TEXTURE_2D, textures[i].id));
    }
    gldc(glActiveTexture(GL_TEXTURE0));

    gldc(glBindVertexArray(_vao));
    gldc(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
    gldc(glBindVertexArray(0));
}

void AssimpMesh::setup_mesh()
{
    gldc(glGenVertexArrays(1, &_vao));
    gldc(glGenBuffers(1, &_vbo));
    gldc(glGenBuffers(1, &_ebo));

    gldc(glBindVertexArray(_vao));

    gldc(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
    gldc(glBufferData(
        GL_ARRAY_BUFFER, vertices.size() * sizeof(AssimpVertex), &vertices[0], GL_STATIC_DRAW));

    gldc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
    gldc(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      indices.size() * sizeof(unsigned int),
                      &indices[0],
                      GL_STATIC_DRAW));

    gldc(glEnableVertexAttribArray(0));
    gldc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), (void*)0));

    gldc(glEnableVertexAttribArray(1));
    gldc(glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), (void*)(offsetof(AssimpVertex, normal))));

    gldc(glEnableVertexAttribArray(2));
    gldc(glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), (void*)(offsetof(AssimpVertex, uv))));

    gldc(glBindVertexArray(0));
}

AssimpModel::AssimpModel(const std::string& file_path) { load_model(file_path); }

void AssimpModel::draw(Shader& shader)
{
    for (auto mesh : _meshes)
    {
        mesh.draw(shader);
    }
}

void AssimpModel::load_model(std::string file_path)
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

void AssimpModel::process_node(aiNode* node, const aiScene* scene)
{
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

AssimpMesh AssimpModel::process_mesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<AssimpVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<AssimpTexture> textures;

    for (size_t i = 0; i < mesh->mNumVertices; i += 1)
    {
        AssimpVertex vertex;
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

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<AssimpTexture> diffuse_maps =
            load_textures_for(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        std::vector<AssimpTexture> specular_maps =
            load_textures_for(material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return AssimpMesh{vertices, indices, textures};
}

std::vector<AssimpTexture>
AssimpModel::load_textures_for(aiMaterial* material, aiTextureType type, std::string type_name)
{
    std::vector<AssimpTexture> textures;

    for (size_t i = 0; i < material->GetTextureCount(type); i += 1)
    {
        aiString str;
        material->GetTexture(type, i, &str);
        AssimpTexture texture;
        texture.id = tex_from_file(str.C_Str(), _directory);
        texture.type = type_name;
        texture.path = std::string(str.C_Str());
        textures.push_back(texture);
    }

    return textures;
}

unsigned int AssimpModel::tex_from_file(const char* path, const std::string& directory)
{
    std::string model_file_path = std::string(path);
    std::string filename = model_file_path.substr(model_file_path.find_last_of("/\\") + 1);
    std::string actual_filepath = directory + '/' + filename;

    EO_LOG_INFO("Loading texture " << actual_filepath);

    unsigned int texture_id;
    gldc(glGenTextures(1, &texture_id));

    int width, height, component_count;
    unsigned char* data = stbi_load(actual_filepath.c_str(), &width, &height, &component_count, 0);
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
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return texture_id;
}
} // namespace eo

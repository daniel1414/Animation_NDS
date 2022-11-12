#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    std::vector<int> BoneIDs;
    std::vector<float> BoneWeights;
};

struct Texture
{
    unsigned int id;
    std::string type;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    uint32_t glBeginParam;

    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh() {}
};

class Model
{
public:
    Model(const std::string& path)
    {
        loadModel(path);
    }

    bool HasAnimations() const;

private:
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene, int indent);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    void printBoneOffsetMatrix(aiBone* Bone);
    void printMatrix(const aiMatrix4x4& m, int indent);
    void parseNodeHierarchy(const aiScene* scene);
    void parseNode(const aiNode* Node, int indent);

    // todo: textures? here?
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    bool m_HasAnimations = false;

    friend class ModelConverter;
};

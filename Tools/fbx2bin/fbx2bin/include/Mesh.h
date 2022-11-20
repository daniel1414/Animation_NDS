#pragma once

#include <string>
#include <vector>
#include <map>

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
    Model(const std::string& path);

    bool HasAnimations() const;

    void GetBoneTransforms(const float TimeS, std::vector<aiMatrix4x4>& Transforms);

private:
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene, int indent);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    void printBoneOffsetMatrix(aiBone* Bone);
    void printMatrix(const aiMatrix4x4& m, int indent);
    void ReadNodeHierarchy(const float TimeInTicks, const aiNode* pNode, const aiMatrix4x4& ParentTransform);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName);

    uint32_t FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    uint32_t FindRotation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    uint32_t FindTranslation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedTranslation(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);

    // todo: textures? here?
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    bool m_HasAnimations = false;
    std::map<std::string, uint32_t> m_BoneNameToIndex;

    Assimp::Importer m_importer;
    const aiScene* m_scene;
    struct BoneInfo
    {
        aiMatrix4x4 OffsetMatrix;
        aiMatrix4x4 FinalTransformation;
        BoneInfo(const aiMatrix4x4& Offset)
        {
            OffsetMatrix = Offset;
        }
    };
    std::vector<BoneInfo> m_BoneInfo;

    aiMatrix4x4 m_GlobalInverseTransform;

    friend class ModelConverter;
};

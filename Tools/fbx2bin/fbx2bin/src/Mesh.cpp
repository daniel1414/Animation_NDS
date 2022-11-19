#include "Mesh.h"

#include <iostream>
#include <iomanip>

#include "Defines.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
    : vertices(vertices), indices(indices), textures(textures)
{
}

Model::Model(const std::string& path)
{
    loadModel(path);
}

bool Model::HasAnimations() const
{
    return m_HasAnimations;
}

void Model::GetBoneTransforms(std::vector<aiMatrix4x4>& Transforms)
{
    Transforms.clear();
    Transforms.reserve(m_BoneInfo.size());

    for (const BoneInfo& Bone : m_BoneInfo)
    {
        Transforms.push_back(Bone.FinalTransformation);
    }
}

void Model::loadModel(const std::string& path)
{
    const aiScene* scene = m_importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << m_importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    if (scene->mNumAnimations > 0)
    {
        m_HasAnimations = true;
    }

    processNode(scene->mRootNode, scene, 0);

    aiMatrix4x4 Matrix;
    if (!Matrix.IsIdentity())
    {
        std::cout << "dupa" << std::endl;
    }

    ReadNodeHierarchy(scene->mRootNode, Matrix);
}

void Model::processNode(aiNode* node, const aiScene* scene, int indent)
{
    if (!node)
    {
        return;
    }

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    std::string indentString;
    indentString.resize(indent, ' ');

    printf("%s-- NODE %s --\n", indentString.c_str(), node->mName.C_Str());
    printMatrix(node->mTransformation, indent);
    std::cout << std::endl;
    for (unsigned int child = 0; child < node->mNumChildren; child++)
    {
        processNode(node->mChildren[child], scene, indent + 4);
        std::cout << std::endl;
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    Mesh Result;

    Result.glBeginParam = mesh->mFaces->mNumIndices == 3 ? GL_TRIANGLES : GL_QUADS;

    // For every vertex
    for (unsigned int VertexID = 0; VertexID < mesh->mNumVertices; VertexID++)
    {
        // process vertex positions, normals and texture coordinates
        Vertex vertex;
        vertex.Position = glm::vec3{ mesh->mVertices[VertexID].x, mesh->mVertices[VertexID].y, mesh->mVertices[VertexID].z };
        vertex.Normal = glm::vec3{ mesh->mNormals[VertexID].x, mesh->mNormals[VertexID].y, mesh->mNormals[VertexID].z };

        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoords = glm::vec2{ mesh->mTextureCoords[0][VertexID].x, mesh->mTextureCoords[0][VertexID].y };
        }
        else
        {
            vertex.TexCoords = glm::vec2{ 0.0f, 0.0f };
        }

        // For each bone - Such bad code, please change in the future
        //for (unsigned int BoneID = 0; BoneID < mesh->mNumBones; BoneID++)
        //{
        //    aiBone* Bone = mesh->mBones[BoneID];

        //    // For each bone weight
        //    for (unsigned int WeightID = 0; WeightID < Bone->mNumWeights; WeightID++)
        //    {
        //        aiVertexWeight& Weight = Bone->mWeights[WeightID];

        //        if (Weight.mVertexId == VertexID)
        //        {
        //            vertex.BoneIDs.push_back(BoneID);
        //            vertex.BoneWeights.push_back(Weight.mWeight);
        //            std::cout << "Vertex " << VertexID << " affectex by bone " << BoneID << " (" << Bone->mName.C_Str() << ")" << " with weight " << Weight.mWeight << "\n";
        //        }
        //    }
        //}

        Result.vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            Result.indices.push_back(face.mIndices[j]);
    }

    // Process bones
    for (unsigned int BoneID = 0; BoneID < mesh->mNumBones; BoneID++)
    {
        aiBone* Bone = mesh->mBones[BoneID];

        m_BoneNameToIndex[Bone->mName.C_Str()] = BoneID;
        m_BoneInfo.push_back(Bone->mOffsetMatrix);

        //printBoneOffsetMatrix(Bone);

        // For each bone weight
        for (unsigned int WeightID = 0; WeightID < Bone->mNumWeights; WeightID++)
        {
            aiVertexWeight& Weight = Bone->mWeights[WeightID];

            Result.vertices[Weight.mVertexId].BoneIDs.push_back(BoneID);
            Result.vertices[Weight.mVertexId].BoneWeights.push_back(Weight.mWeight);
            //std::cout << "Vertex " << Weight.mVertexId << " affected by bone " << BoneID << " (" << Bone->mName.C_Str() << ")" << " with weight " << Weight.mWeight << "\n";
        }
    }
    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        //aiMaterial* Material = scene->mMaterials[mesh->mMaterialIndex];
        //aiString texture_file;

        //Material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);
        //
        //if (const aiTexture* texture = scene->GetEmbeddedTexture(texture_file.C_Str()))
        //{
        //    //returned pointer is not null, read texture from memory
        //    for (int i = 0; i < texture->mWidth; i++)
        //    {
        //        aiColor4D color = texture->pcData[i];
        //        std::cout << color.r << " " << color.g << " " << color.b << " ";
        //        if (i % 4 == 0)
        //        {
        //            std::cout << "\n";
        //        }
        //    }
        //}
        //else
        //{
        //    //regular file, check if it exists and read it
        //    int x = 0;
        //}
    }

    //parseNodeHierarchy(scene);

    return Result;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    return {};
}

void Model::printBoneOffsetMatrix(aiBone* Bone)
{
    aiMatrix4x4& m = Bone->mOffsetMatrix;

    std::cout << "Bone " << Bone->mName.C_Str() << " offset matrix:\n";
    printMatrix(m, 4);
}

void Model::printMatrix(const aiMatrix4x4& m, int indent)
{
    std::string indentString;
    indentString.resize(indent, ' ');

    printf("%s%.4f %.4f %.4f %.4f\n", indentString.c_str(), m.a1, m.a2, m.a3, m.a4);
    printf("%s%.4f %.4f %.4f %.4f\n", indentString.c_str(), m.b1, m.b2, m.b3, m.b4);
    printf("%s%.4f %.4f %.4f %.4f\n", indentString.c_str(), m.c1, m.c2, m.c3, m.c4);
    printf("%s%.4f %.4f %.4f %.4f\n", indentString.c_str(), m.d1, m.d2, m.d3, m.d4);
}

void Model::ReadNodeHierarchy(const aiNode* pNode, const aiMatrix4x4& ParentTransform)
{
    std::string NodeName = pNode->mName.C_Str();
    aiMatrix4x4 NodeTransformation = pNode->mTransformation;
    aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;

    if (m_BoneNameToIndex.find(NodeName) != m_BoneNameToIndex.end())
    {
        uint32_t BoneIndex = m_BoneNameToIndex[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation = GlobalTransformation * m_BoneInfo[BoneIndex].OffsetMatrix;
        //printf("Bone %s\n", NodeName.c_str());
        //printMatrix(m_BoneInfo[BoneIndex].FinalTransformation, 4);
    }

    for (uint32_t i = 0; i < pNode->mNumChildren; i++)
    {
        ReadNodeHierarchy(pNode->mChildren[i], GlobalTransformation);
    }
}

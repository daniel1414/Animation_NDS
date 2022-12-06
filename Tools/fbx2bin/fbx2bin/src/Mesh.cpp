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

void Model::GetBoneTransforms(const float TimeS, std::vector<aiMatrix4x4>& Transforms, uint32_t& NumBones, uint32_t& TicksPerSecond, uint32_t& AnimationTicks)
{
    TicksPerSecond = static_cast<uint32_t>(m_scene->mAnimations[0]->mTicksPerSecond != 0.0f ? m_scene->mAnimations[0]->mTicksPerSecond : 30);
    AnimationTicks = static_cast<uint32_t>(m_scene->mAnimations[0]->mDuration);
    const float TimeInTicks = TimeS * TicksPerSecond;
    //const float AnimationTimeTicks = fmod(TimeInTicks, static_cast<float>(m_scene->mAnimations[0]->mDuration));

    Transforms.clear();
    Transforms.reserve(m_scene->mAnimations[0]->mNumChannels * AnimationTicks);

    for (unsigned int tick = 0; tick < AnimationTicks; tick++)
    {
        aiMatrix4x4 Matrix;
        ReadNodeHierarchy(static_cast<float>(tick), m_scene->mRootNode, Matrix);

        for (const BoneInfo& Bone : m_BoneInfo)
        {
            Transforms.push_back(Bone.FinalTransformation);
        }
    }

    NumBones = m_BoneInfo.size();
}

void Model::loadModel(const std::string& path)
{
    const aiScene* scene = m_importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
    m_scene = scene;

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << m_importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    if (scene->mNumAnimations > 0)
    {
        m_HasAnimations = true;
        m_GlobalInverseTransform = scene->mRootNode->mTransformation;
        m_GlobalInverseTransform.Inverse();
    }

    processNode(scene->mRootNode, scene, 0);
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

    //printf("%s-- NODE %s --\n", indentString.c_str(), node->mName.C_Str());
    //printMatrix(node->mTransformation, indent);
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

void Model::ReadNodeHierarchy(const float TimeInTicks, const aiNode* pNode, const aiMatrix4x4& ParentTransform)
{
    std::string NodeName = pNode->mName.C_Str();

    const aiAnimation* pAnimation = m_scene->mAnimations[0];

    aiMatrix4x4 NodeTransformation = pNode->mTransformation;

    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

    if (pNodeAnim)
    {
        aiVector3D Scaling(1.0f, 1.0f, 1.0f);
        //CalcInterpolatedScaling(Scaling, TimeInTicks, pNodeAnim);
        aiMatrix4x4 ScalingM;
        ScalingM.Scaling(Scaling, ScalingM);

        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, TimeInTicks, pNodeAnim);
        aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

        aiVector3D Translation;
        CalcInterpolatedTranslation(Translation, TimeInTicks, pNodeAnim);
        aiMatrix4x4 TranslationM;
        TranslationM.Translation(Translation, TranslationM);

        NodeTransformation = TranslationM * RotationM * ScalingM;

        //printf("Animated node %s Tick %f\n", NodeName.c_str(), TimeInTicks);
        //printMatrix(NodeTransformation, 4);
    }

    aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;
    if (m_BoneNameToIndex.find(NodeName) != m_BoneNameToIndex.end())
    {
        uint32_t BoneIndex = m_BoneNameToIndex[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].OffsetMatrix;
        //printf("Bone %s\n", NodeName.c_str());
        //printMatrix(m_BoneInfo[BoneIndex].FinalTransformation, 4);
    }

    for (uint32_t i = 0; i < pNode->mNumChildren; i++)
    {
        ReadNodeHierarchy(TimeInTicks, pNode->mChildren[i], GlobalTransformation);
    }
}

const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName)
{
    for (uint32_t i = 0; i < pAnimation->mNumChannels; i++)
    {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if (std::string(pNodeAnim->mNodeName.C_Str()) == NodeName)
        {
            return pNodeAnim;
        }
    }
    return nullptr;
}

uint32_t Model::FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (uint32_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
    {
        float t = static_cast<float>(pNodeAnim->mScalingKeys[i + 1].mTime);
        if (AnimationTimeTicks < t)
        {
            return i;
        }
    }

    return 0;
}

uint32_t Model::FindRotation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint32_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
    {
        float t = static_cast<float>(pNodeAnim->mRotationKeys[i + 1].mTime);
        if (AnimationTimeTicks < t)
        {
            return i;
        }
    }
    return 0;
}

uint32_t Model::FindTranslation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumPositionKeys > 0);

    for (uint32_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
    {
        float t = static_cast<float>(pNodeAnim->mPositionKeys[i + 1].mTime);
        if (AnimationTimeTicks < t)
        {
            return i;
        }
    }
    return 0;
}

void Model::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1)
    {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint32_t ScalingIndex = FindScaling(AnimationTimeTicks, pNodeAnim);
    uint32_t NextScalingIndex = ScalingIndex + 1;
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float t1 = static_cast<float>(pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float t2 = static_cast<float>(pNodeAnim->mScalingKeys[NextScalingIndex].mTime);
    float DeltaTime = t2 - t1;
    float Factor = (AnimationTimeTicks - t1) / DeltaTime;
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

void Model::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumRotationKeys == 1)
    {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    uint32_t RotationIndex = FindRotation(AnimationTimeTicks, pNodeAnim);
    uint32_t NextRotationIndex = RotationIndex + 1;
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float t1 = static_cast<float>(pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float t2 = static_cast<float>(pNodeAnim->mRotationKeys[NextRotationIndex].mTime);
    float DeltaTime = t2 - t1;
    float Factor = (AnimationTimeTicks - t1) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& Start = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& End = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, Start, End, Factor);
    Out = Start;
    Out.Normalize();
}

void Model::CalcInterpolatedTranslation(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1)
    {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    uint32_t PositionIndex = FindTranslation(AnimationTimeTicks, pNodeAnim);
    uint32_t NextPositionIndex = PositionIndex + 1;
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float t1 = static_cast<float>(pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float t2 = static_cast<float>(pNodeAnim->mPositionKeys[NextPositionIndex].mTime);
    float DeltaTime = t2 - t1;
    float Factor = (AnimationTimeTicks - t1) / DeltaTime;
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

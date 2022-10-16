#include "Mesh.h"

#include <iostream>

#include "Defines.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
    : vertices(vertices), indices(indices), textures(textures)
{
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    Mesh Result;

    Result.glBeginParam = mesh->mFaces->mNumIndices == 3 ? GL_TRIANGLES : GL_QUADS;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // process vertex positions, normals and texture coordinates
        Vertex vertex;
        vertex.Position = glm::vec3{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        vertex.Normal = glm::vec3{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoords = glm::vec2{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        }
        else
        {
            vertex.TexCoords = glm::vec2{ 0.0f, 0.0f };
        }
        
        Result.vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            Result.indices.push_back(face.mIndices[j]);
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

    return Result;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    return {};
}

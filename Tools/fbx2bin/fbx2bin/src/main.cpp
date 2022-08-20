#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "InputArguments.h"


int main(int argc, char** argv)
{
    InputArguments inputArguments(argc, argv);

    if (!inputArguments.IsValid())
    {
        std::cout << "ERROR::" << "Input arguments invalid!" << std::endl;
        return -1;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(inputArguments.SourceFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return -1;
    }

    return 0;
}
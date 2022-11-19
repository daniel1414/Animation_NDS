#pragma once

#include "Mesh.h"

class ModelConverter
{
    Model& model;
public:

    ModelConverter(Model& model) : model(model) {}

    uint32_t* GetNintendoMeshes(uint32_t& OutSize);

    uint32_t* GetAnimationData(uint32_t& OutSize);

private:

    struct VertexInfo
    {
        uint32_t PositionOffsetInMemory;
        uint8_t BoneIDs[4];
        uint32_t BoneWeights[4];
        
        VertexInfo(uint32_t Offset) : PositionOffsetInMemory(Offset) 
        {
            memset(BoneIDs, 0, 4 * sizeof(uint8_t));
            memset(BoneWeights, 0, 4 * sizeof(uint32_t));
        }
    };

    std::vector<VertexInfo> m_VertexInfos;

    struct BoneInfo
    {
        int32_t m[16]; // Final Transform

        BoneInfo()
        {
            memset(m, 0, 16 * sizeof(int32_t));
        }
    };

    std::vector<BoneInfo> m_BoneInfos;
};
#include "AnimatedModel.h"

#include "Renderer.h"
#include "Log.h"

typedef short int vx10;        /*!< \brief vertex 4.6 fixed point, USED FOR 10bit VERTICES!!! MAX 7.984375, MIN -8.0*/
#define inttovx10(n)          ((n) << 6) /*!< \brief convert int to vx10 */
#define f32tovx10(n)          ((n) >> 6) /*!< \brief f32 to vx10 */
#define vx10toint(n)          ((n) >> 6) /*!< \brief convert vx10 to int */
#define vx10tof32(n)          ((n) << 6) /*!< \brief convert vx10 to f32 */
#define vx10tofloat(n)        ((n) / (float)(1 << 6)) /*!< \brief convert vx10 to float */
#define floattovx10(n)        ((vx10)((n) * (1 << 6))) /*!< \brief convert float to vx10 */
#define VERTEXX10_PACK(x,y,z) (uint32_t)(((x) & 0x3FF) | ((y & 0x3FF) << 10) | ((z & 0x3FF) << 20)) /*!< \brief Pack to v16 values into one 32bit value */

AnimatedModel::AnimatedModel(const void* model, const size_t modelSize, const void* animData, const size_t animDataSize)
    : modelPtr(model), modelSize(modelSize), animDataPtr(animData), animDataSize(animDataSize)
{
}

void AnimatedModel::Draw(const float Time)
{
    // Get a copy of the model in order to modify the vertex positions
    uint32_t* modelData = (uint32_t*)malloc(modelSize);
    memcpy(modelData, modelPtr, modelSize);

    // Get the animation time (for now leave at 0.0)

    // Get the bone transforms that relate to the time

    // Multiply the vertex coodrinated by the bone matrices

    //const VertexInfo* AnimData = reinterpret_cast<const VertexInfo*>(animDataPtr);
    const uint32_t* AnimData = reinterpret_cast<const uint32_t*>(animDataPtr);

    // Offset in words
    const uint32_t VertexDataOffset = (static_cast<const uint32_t*>(animDataPtr))[VertexDataPosition];
    const uint32_t BoneDataOffset = (static_cast<const uint32_t*>(animDataPtr))[BoneDataPosition];

    const uint32_t VertexInfoCount = (BoneDataOffset - VertexDataOffset) * 4 / sizeof(VertexInfo);
    const uint32_t BoneInfoCount = (animDataSize / 4 - BoneDataOffset) * 4 / sizeof(BoneInfo);

    const VertexInfo* VertexInfoBegin = reinterpret_cast<const VertexInfo*>(&AnimData[VertexDataOffset]);
    const BoneInfo* BoneInfoBegin = reinterpret_cast<const BoneInfo*>(&AnimData[BoneDataOffset]);

    // Multiply the vertex position by the final Bone transform
    for(uint32_t VertexInfoID = 0; VertexInfoID < VertexInfoCount; VertexInfoID++)
    {
        const VertexInfo& vertexInfo = VertexInfoBegin[VertexInfoID];
        uint32_t& VertexPosition = modelData[vertexInfo.PositionOffsetInMemory];

        // Multiply the vertex position by the weighted average of the bone transforms
        Mat4x4 FinalVertexTransformation;
        for(uint32_t i = 0; i < 4; i++)
        {
            const BoneInfo& boneInfo = BoneInfoBegin[vertexInfo.BoneID[i]];
            // Bone weight in f32 already
            if(vertexInfo.BoneWeight[i] != 0)
            {
                FinalVertexTransformation = FinalVertexTransformation * boneInfo.FinalTransform * vertexInfo.BoneWeight[i];
            }
        }

        Vec4 VertexPositionf32;
        VertexPositionf32.x = vx10tof32(VertexPosition & 0x3FF);
        VertexPositionf32.y = vx10tof32((VertexPosition >> 10) & 0x3FF);
        VertexPositionf32.z = vx10tof32((VertexPosition >> 20) & 0x3FF);
        VertexPositionf32.w = inttof32(1);

        Vec4 FinalVertexPositionf32 = FinalVertexTransformation * VertexPositionf32;
        VertexPosition = VERTEXX10_PACK(f32tovx10(FinalVertexPositionf32.x), f32tovx10(FinalVertexPositionf32.y), f32tovx10(FinalVertexPositionf32.z));
    }

    //LogAnimationData();
    Renderer::drawModel((void*)modelData);

    free(modelData);
}

void AnimatedModel::LogAnimationData() const
{
    const uint32_t* AnimData = reinterpret_cast<const uint32_t*>(animDataPtr);

    LOG("\n -- General Model Info --\n", 0);

    // Offset in words
    const uint32_t VertexDataOffset = (static_cast<const uint32_t*>(animDataPtr))[VertexDataPosition];
    LOG("VertexDataOffset: %d", VertexDataOffset);

    // Offset in words
    const uint32_t BoneDataOffset = (static_cast<const uint32_t*>(animDataPtr))[BoneDataPosition];
    LOG("BoneDataOffset: %d", BoneDataOffset);
    
    const uint32_t VertexInfoCount = (BoneDataOffset - VertexDataOffset) * 4 / sizeof(VertexInfo);
    LOG("VertexInfoCount: %d", VertexInfoCount);

    const uint32_t BoneInfoCount = (animDataSize / 4 - BoneDataOffset) * 4 / sizeof(BoneInfo);
    LOG("BoneInfoCount: %d\n", BoneInfoCount);

    // Log vertex info
    const VertexInfo* VertexInfoBegin = reinterpret_cast<const VertexInfo*>(&AnimData[VertexDataOffset]);
    for(uint32_t VertexInfoID = 0; VertexInfoID < VertexInfoCount; VertexInfoID++)
    {
        LOG("-- VertexInfoID %d --", VertexInfoID);
        LOG("Position %d", VertexInfoBegin[VertexInfoID].PositionOffsetInMemory);
        LOG("Bone0 %d", VertexInfoBegin[VertexInfoID].BoneID[0]);
        LOG("Bone1 %d", VertexInfoBegin[VertexInfoID].BoneID[1]);
        LOG("Bone2 %d", VertexInfoBegin[VertexInfoID].BoneID[2]);
        LOG("Bone3 %d", VertexInfoBegin[VertexInfoID].BoneID[3]);
        LOG("Weight0 %d", VertexInfoBegin[VertexInfoID].BoneWeight[0]);
        LOG("Weight1 %d", VertexInfoBegin[VertexInfoID].BoneWeight[1]);
        LOG("Weight2 %d", VertexInfoBegin[VertexInfoID].BoneWeight[2]);
        LOG("Weight3 %d\n", VertexInfoBegin[VertexInfoID].BoneWeight[3]);
    }

    // Log bone info
    const BoneInfo* BoneInfoBegin = reinterpret_cast<const BoneInfo*>(&AnimData[BoneDataOffset]);
    for(uint32_t BoneInfoID = 0; BoneInfoID < BoneInfoCount; BoneInfoID++)
    {
        LOG("-- BoneInfoID %d --", BoneInfoID);
        Logger::LogMatrix4x4(*(m4x4*)&BoneInfoBegin[BoneInfoID].FinalTransform);
        LOG("\n", 0);
    }
}
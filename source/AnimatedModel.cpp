#include "AnimatedModel.h"

#include "Renderer.h"
#include "Log.h"

typedef short int vx10;        /*!< \brief vertex 4.6 fixed point, USED FOR 10bit VERTICES!!! MAX 7.984375, MIN -8.0*/
#define inttovx10(n)          ((n) * (1 << 6)) /*!< \brief convert int to vx10 */
#define vx10toint(n)          ((n) / (1 << 6)) /*!< \brief convert vx10 to int */
#define f32tovx10(n)          ((vx10)(n >> 6)) /*!< \brief f32 to vx10 */
#define vx10tof32(n)          ((((int32)n) & (1 << 9) ? (((0xFFFFFC00 | (int32)n ) << 6) & 0x2F) : (((int32)n) << 6))) /*!< \brief convert vx10 to f32 */
#define vx10tofloat(n)        ((float)(n) / (float)(1 << 6)) /*!< \brief convert vx10 to float */
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

    const uint32_t* AnimData = reinterpret_cast<const uint32_t*>(animDataPtr);
    const AnimationInfoHeader* Header = reinterpret_cast<const AnimationInfoHeader*>(animDataPtr);

    const uint32_t CurrentAnimationTick = static_cast<uint32_t>(Time * Header->AnimationTicksPerSecond) % Header->AnimationTicks;

    // Offset in words
    const uint32_t VertexDataOffset = Header->VertexDataPosition;
    const uint32_t BoneDataOffset = Header->BoneDataPosition;

    const uint32_t VertexInfoCount = (BoneDataOffset - VertexDataOffset) * 4 / sizeof(VertexInfo);
    const uint32_t BoneInfoCount = (animDataSize / 4 - BoneDataOffset) * 4 / sizeof(BoneInfo);

    const VertexInfo* VertexInfoBegin = reinterpret_cast<const VertexInfo*>(&AnimData[VertexDataOffset]);

    // Move the bone begin ptr to the current animation tick
    const BoneInfo* BoneInfoBegin = reinterpret_cast<const BoneInfo*>(&AnimData[BoneDataOffset]) + CurrentAnimationTick * Header->BoneCount;

    // Multiply the vertex position by the final Bone transform
    for(uint32_t VertexInfoID = 0; VertexInfoID < VertexInfoCount; VertexInfoID++)
    {
        const VertexInfo& vertexInfo = VertexInfoBegin[VertexInfoID];
        uint32_t& VertexPosition = modelData[vertexInfo.PositionOffsetInMemory];

        // Multiply the vertex position by the weighted average of the bone transforms
        Mat4x4 FinalVertexTransformation;
        FinalVertexTransformation.Zero();
        for(uint32_t i = 0; i < 4 && i < Header->BoneCount; i++)
        {
            const BoneInfo& boneInfo = BoneInfoBegin[vertexInfo.BoneID[i]];
            // Bone weight in f32 already
            if(vertexInfo.BoneWeight[i] != 0)
            {
                FinalVertexTransformation = FinalVertexTransformation + boneInfo.FinalTransform * vertexInfo.BoneWeight[i];
            }
        }

        vx10 x = (VertexPosition & 0x3FF);
        vx10 y = ((VertexPosition >> 10) & 0x3FF);
        vx10 z = ((VertexPosition >> 20) & 0x3FF);

        if(x & (1 << 9))
            x |= 0xFC00;
        if(y & (1 << 9))
            y |= 0xFC00;
        if(z & (1 << 9))
            z |= 0xFC00;

        Vec4 VertexPositionf32;
        VertexPositionf32.x = floattof32(vx10tofloat(x));
        VertexPositionf32.y = floattof32(vx10tofloat(y));
        VertexPositionf32.z = floattof32(vx10tofloat(z));
        VertexPositionf32.w = inttof32(1);

        LOG("\nCalculating vertex %d in tick %d", VertexInfoID, CurrentAnimationTick);
        LOG("Original Vertex Position (%f,%f,%f)", f32tofloat(VertexPositionf32.x), f32tofloat(VertexPositionf32.y), f32tofloat(VertexPositionf32.z));
        LOG("FinalVertexTransformation", 0);
        //Logger::LogMatrix4x4(*(m4x4*)&FinalVertexTransformation);

        Vec4 FinalVertexPositionf32 = FinalVertexTransformation * VertexPositionf32;
        LOG("Final Vertex Position (%f,%f,%f)", f32tofloat(FinalVertexPositionf32.x), f32tofloat(FinalVertexPositionf32.y), f32tofloat(FinalVertexPositionf32.z));
        VertexPosition = VERTEXX10_PACK(f32tovx10(FinalVertexPositionf32.x), f32tovx10(FinalVertexPositionf32.y), f32tovx10(FinalVertexPositionf32.z));
        //VertexPosition = VERTEXX10_PACK(f32tovx10(Positionf32.x), f32tovx10(Positionf32.y), f32tovx10(Positionf32.z));
    }

    Renderer::drawModel((void*)modelData);

    free(modelData);
}

void AnimatedModel::LogAnimationData() const
{
    const uint32_t* AnimData = reinterpret_cast<const uint32_t*>(animDataPtr);
    const AnimationInfoHeader* Header = reinterpret_cast<const AnimationInfoHeader*>(animDataPtr);
    
    LOG("\n -- General Model Info --\n", 0);

    // File size in bytes
    LOG("AnimationDataSize: %d", animDataSize);

    // Offset in words
    const uint32_t VertexDataOffset = Header->VertexDataPosition;
    LOG("VertexDataOffset: %d", VertexDataOffset);

    // Size in bytes
    const uint32_t VertexDataSize = Header->VertexDataSize;
    LOG("VertexDataSize: %d", VertexDataSize);

    // Offset in words
    const uint32_t BoneDataOffset = Header->BoneDataPosition;
    LOG("BoneDataOffset: %d", BoneDataOffset);

    const uint32_t BoneCount = Header->BoneCount;
    LOG("BoneCount: %d", BoneCount);

    const uint32_t AnimationTicksPerSecond = Header->AnimationTicksPerSecond;
    LOG("AnimationTicksPerSecond: %d", AnimationTicksPerSecond);

    const uint32_t AnimationTicks = Header->AnimationTicks;
    LOG("AnimationTicks: %d", AnimationTicks);
    
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
        LOG("Weight0 %f", f32tofloat(VertexInfoBegin[VertexInfoID].BoneWeight[0]));
        LOG("Weight1 %f", f32tofloat(VertexInfoBegin[VertexInfoID].BoneWeight[1]));
        LOG("Weight2 %f", f32tofloat(VertexInfoBegin[VertexInfoID].BoneWeight[2]));
        LOG("Weight3 %f\n", f32tofloat(VertexInfoBegin[VertexInfoID].BoneWeight[3]));
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
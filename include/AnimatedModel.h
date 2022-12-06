#pragma once

#include <cstddef>
#include <stdint.h>
#include <nds/arm9/videoGL.h>

#include "Math.h"


struct AnimationInfoHeader
{
    uint32_t VertexDataPosition;
    uint32_t VertexDataSize;
    uint32_t BoneCount;
    uint32_t BoneDataPosition;
    uint32_t AnimationTicksPerSecond;
    uint32_t AnimationTicks;
};

struct VertexInfo
{
    // Position offset in words
    uint32_t PositionOffsetInMemory;
    uint8_t BoneID[4];
    uint32_t BoneWeight[4];
};

struct BoneInfo
{
    Mat4x4 FinalTransform;
};

class AnimatedModel
{
public:
    AnimatedModel(const void* model, const size_t modelSize, const void* animData, const size_t animDataSize);

    void LogAnimationData() const;
    void Draw(const float Time);

private:
    const void* modelPtr;
    const size_t modelSize;
    const void* animDataPtr;
    const size_t animDataSize;

    void LogMatrix4x4() const;
};
#pragma once

#include <cstddef>
#include <stdint.h>
#include <nds/arm9/videoGL.h>

#include "Math.h"

/* Enum that holds information about sections of information in AnimDataPtr */
enum EAnimationInfoPosition
{
    VertexDataPosition = 0,
    BoneDataPosition = 1,
    AnimationDataPosition = 2
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

struct AnimationData
{

};

class AnimatedModel
{
public:
    AnimatedModel(const void* model, const size_t modelSize, const void* animData, const size_t animDataSize);

    void Draw(const float Time);

private:
    const void* modelPtr;
    const size_t modelSize;
    const void* animDataPtr;
    const size_t animDataSize;

    void LogAnimationData() const;
    void LogMatrix4x4() const;
};
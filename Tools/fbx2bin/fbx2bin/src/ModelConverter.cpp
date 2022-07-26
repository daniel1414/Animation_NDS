#include "ModelConverter.h"

#include "Defines.h"

uint32_t* ModelConverter::GetNintendoMeshes(uint32_t& OutSize)
{
    // one byte for storring the length of the data
    OutSize = 1;

    // two bytes for begin commands (FIFO pack and draw mode
    uint8_t BeginCommandSize = 2;
    OutSize += BeginCommandSize;

    uint32_t BytesPerIndex = 4;

    // FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_COLOR, FIFO_VERTEX10, FIFO_NOP);
    for (const Mesh& mesh : model.meshes)
    {
        OutSize += mesh.indices.size() * BytesPerIndex;
    }

    // Allocate a buffer for model data
    uint32_t* Data = (uint32_t*)malloc(sizeof(uint32_t) * OutSize);
    int SPR = 0;
    Data[SPR++] = OutSize - 1;

    // fill initial commands
    Data[SPR++] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_NOP, FIFO_NOP, FIFO_NOP);
    Data[SPR++] = model.meshes[0].glBeginParam;

    for (const Mesh& mesh : model.meshes)
    {
        for (unsigned int index : mesh.indices)
        {
            // command pack
            Data[SPR++] = FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_TEX_COORD, FIFO_VERTEX10, FIFO_NOP);

            // get the normal of the vertex
            vn10 nx = floattovn10(mesh.vertices[index].Normal.x);
            vn10 ny = floattovn10(mesh.vertices[index].Normal.y);
            vn10 nz = floattovn10(mesh.vertices[index].Normal.z);
            Data[SPR++] = NORMAL_PACK(nx, ny, nz);

            // fuck the vertex color, texture is more important
            Data[SPR++] = TEXTURE_PACK(floattot16(mesh.vertices[index].TexCoords.x * 256.0f), floattot16(mesh.vertices[index].TexCoords.y * 256.0f));

            // get the vertex position of the vertex
            v10 x = floattov10(mesh.vertices[index].Position.x);
            v10 y = floattov10(mesh.vertices[index].Position.y);
            v10 z = floattov10(mesh.vertices[index].Position.z);

            // Add the vertex poisition to an array
            VertexInfo vertexInfo(SPR);
            for (uint32_t i = 0; i < mesh.vertices[index].BoneIDs.size() && i < 4; i++)
            {
                // Leave the bone ID as a uint8_t
                vertexInfo.BoneIDs[i] = mesh.vertices[index].BoneIDs[i];
            }
            for (uint32_t i = 0; i < mesh.vertices[index].BoneWeights.size() && i < 4; i++)
            {
                // convert the bone weight from float to f32
                vertexInfo.BoneWeights[i] = floattof32(mesh.vertices[index].BoneWeights[i]);
            }
            m_VertexInfos.push_back(vertexInfo);
            Data[SPR++] = VERTEX10_PACK(x, y, z);
        }
    }

    return Data;
}

uint32_t* ModelConverter::GetAnimationData(uint32_t& OutSize)
{
    // for now
    if (!model.HasAnimations())
    {
        return nullptr;
    }

    FileHeader Header;
    Header.VertexDataPosition = sizeof(FileHeader) / 4;

    // get the bone data
    std::vector<aiMatrix4x4> BoneTransforms;
    model.GetBoneTransforms(0.0f, BoneTransforms, Header.BoneCount, Header.AnimationTicksPerSecond, Header.AnimationTicks);

    // Data layout:
    const size_t HeaderSize = sizeof(FileHeader);
    OutSize = HeaderSize; // for offsets of data into the memory

    // for the vertex data
    const uint32_t VertexInfoCount = m_VertexInfos.size();
    const size_t VertexDataSize = VertexInfoCount * sizeof(VertexInfo);
    OutSize += VertexDataSize;
    Header.VertexDataSize = VertexDataSize;

    // Bone Data
    Header.BoneDataPosition = HeaderSize / 4 + VertexDataSize / 4;
    const uint32_t BoneInfoCount = BoneTransforms.size();
    const uint32_t BoneDataSize = BoneInfoCount * sizeof(BoneInfo);
    OutSize += BoneDataSize;

    // allocate the buffer
    uint32_t* data = (uint32_t*)malloc(OutSize);

    // copy the header
    memcpy(data, &Header, HeaderSize);

    // Fill in the vertex data
    memcpy(&data[Header.VertexDataPosition], m_VertexInfos.data(), VertexDataSize);

    // Fill in the bone data
    for (uint32_t i = 0; i < BoneTransforms.size(); i++)
    {
        const aiMatrix4x4& matrix = BoneTransforms[i];

        BoneInfo boneInfo;
        boneInfo.m[0] = floattof32(matrix.a1);
        boneInfo.m[1] = floattof32(matrix.a2);
        boneInfo.m[2] = floattof32(matrix.a3);
        boneInfo.m[3] = floattof32(matrix.a4);

        boneInfo.m[4] = floattof32(matrix.b1);
        boneInfo.m[5] = floattof32(matrix.b2);
        boneInfo.m[6] = floattof32(matrix.b3);
        boneInfo.m[7] = floattof32(matrix.b4);

        boneInfo.m[8] = floattof32(matrix.c1);
        boneInfo.m[9] = floattof32(matrix.c2);
        boneInfo.m[10] = floattof32(matrix.c3);
        boneInfo.m[11] = floattof32(matrix.c4);

        boneInfo.m[12] = floattof32(matrix.d1);
        boneInfo.m[13] = floattof32(matrix.d2);
        boneInfo.m[14] = floattof32(matrix.d3);
        boneInfo.m[15] = floattof32(matrix.d4);

        m_BoneInfos.push_back(boneInfo);
    }

    memcpy(&data[Header.BoneDataPosition], m_BoneInfos.data(), BoneDataSize);

    return data;
}

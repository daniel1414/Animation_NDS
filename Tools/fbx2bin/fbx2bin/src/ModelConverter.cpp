#include "ModelConverter.h"

#include "Defines.h"

uint32_t* ModelConverter::ToNintendoInstructions(uint32_t& OutSize)
{
    return ConvertStaticModel(OutSize);
}

uint32_t* ModelConverter::ConvertStaticModel(uint32_t& OutSize)
{
    // one byte for storring the length of the data
    OutSize = 1;

    // two bytes for begin commands
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

    // Buffer for vertex positions
    std::vector<int> VertexPositions;

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
            VertexPositions.push_back(SPR);
            Data[SPR++] = VERTEX10_PACK(x, y, z);
        }
    }

    return Data;
}

uint32_t* ModelConverter::ConvertAnimatedModel(uint32_t& OutSize)
{
    return nullptr;
}

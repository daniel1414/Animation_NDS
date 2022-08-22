#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "InputArguments.h"
#include "Defines.h"

#include "Mesh.h"

#define DataType uint8_t

void drawTriangle(FILE* file)
{
	uint16_t color = 31 << 5;

	uint32_t* fifoData = (uint32_t*)malloc(18 * sizeof(uint32_t));
	fifoData[0] = 17; // length of command list

	// vertices
	fifoData[1] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_NOP, FIFO_NOP, FIFO_NOP);
	fifoData[2] = GL_TRIANGLES;
	fifoData[3] = FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_COLOR, FIFO_VERTEX16, FIFO_NOP);
	fifoData[4] = NORMAL_PACK(0, 0, floattov10(.97)); // point to positive z
	fifoData[5] = color;
	fifoData[6] = VERTEX_PACK(inttov16(1), inttov16(0)); // x and y coordinate
	fifoData[7] = 0; // z coordinate
	fifoData[8] = FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_COLOR, FIFO_VERTEX16, FIFO_NOP);
	fifoData[9] = NORMAL_PACK(0, 0, floattov10(.97)); // point to positive z
	fifoData[10] = color;
	fifoData[11] = VERTEX_PACK(0, inttov16(2));
	fifoData[12] = 0;
	fifoData[13] = FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_COLOR, FIFO_VERTEX16, FIFO_NOP);
	fifoData[14] = NORMAL_PACK(0, 0, floattov10(.97)); // point to positive z
	fifoData[15] = color;
	fifoData[16] = VERTEX_PACK(inttov16(-1), 0);
	fifoData[17] = 0;

	fwrite(fifoData, sizeof(uint32_t), 18, file);
}

int main(int argc, char** argv)
{
    InputArguments inputArguments(argc, argv);

	inputArguments.SourceFilePath = "cube.fbx";
	inputArguments.OutDirectory = "build";
	inputArguments.OutFileName = "cube.fbx.bin";

    if (!inputArguments.IsValid())
    {
        std::cout << "ERROR::" << "Input arguments invalid!" << std::endl;
		return -1;
    }

	Model model(inputArguments.SourceFilePath);

    FILE* file;
    std::string OutFilePath = inputArguments.OutDirectory + "/" + inputArguments.OutFileName;
    fopen_s(&file, OutFilePath.c_str(), "wb");
	if (file == nullptr)
	{
		std::cout << "faied to open file " << OutFilePath << std::endl;
		return -1;
	}
	drawTriangle(file);
    fclose(file);

    return 0;
}
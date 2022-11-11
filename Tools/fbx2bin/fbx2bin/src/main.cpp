#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "InputArguments.h"
#include "Defines.h"

#include "Mesh.h"
#include "ModelConverter.h"

#define DataType uint8_t

void drawTriangle(FILE* file)
{
	uint16_t color = 31;

	uint32_t* fifoData = (uint32_t*)malloc(15 * sizeof(uint32_t));
	fifoData[0] = 14; // length of command list

	// vertices
	fifoData[1] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_NOP, FIFO_NOP, FIFO_NOP);
	fifoData[2] = GL_TRIANGLES;
	fifoData[3] = FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_COLOR, FIFO_VERTEX10, FIFO_NOP);
	fifoData[4] = NORMAL_PACK(0, 0, floattov10(.97)); // point to positive z
	fifoData[5] = color;
	fifoData[6] = VERTEX10_PACK(inttov10(1), inttov10(0), inttov10(0)); // x and y coordinate
	fifoData[7] = FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_COLOR, FIFO_VERTEX10, FIFO_NOP);
	fifoData[8] = NORMAL_PACK(0, 0, floattov10(.97)); // point to positive z
	fifoData[9] = color;
	fifoData[10] = VERTEX10_PACK(0, inttov10(2), 0);
	fifoData[11] = FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_COLOR, FIFO_VERTEX10, FIFO_NOP);
	fifoData[12] = NORMAL_PACK(0, 0, floattov10(.97)); // point to positive z
	fifoData[13] = color;
	fifoData[14] = VERTEX10_PACK(inttov10(-1), 0, 0);

	fwrite(fifoData, sizeof(uint32_t), 18, file);
}

int main(int argc, char** argv)
{
    InputArguments inputArguments(argc, argv);

	if (argc == 1)
	{
		inputArguments.SourceFilePath = "BoxAnim.fbx";
		inputArguments.OutDirectory = "build";
		inputArguments.OutFileName = "BoxAnim.fbx.bin";
	}

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

	ModelConverter mc(model);

	uint32_t size = 0;
	uint32_t* data = mc.ToNintendoInstructions(size);

	//drawTriangle(file);
	
	fwrite(data, sizeof(uint32_t), size, file);

	free(data);

	fclose(file);

    return 0;
}
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <unordered_map>

#include "Defines.h"

#define DataType uint8_t

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N])
{
	return N;
}

template<typename T>
T* AllocBuffer(int ElemCount)
{
	return (T*)malloc(ElemCount * sizeof(T));
}

template<typename T>
void FreeBuffer(T* Buffer)
{
	free(Buffer);
}

int main(int argc, char** argv)
{
	std::unordered_map<DataType, const char*> commands;

	commands[FIFO_NOP] = "FIFO_NOP";
	commands[FIFO_STATUS] = "FIFO_STATUS";
	commands[FIFO_COLOR] = "FIFO_COLOR";

	commands[FIFO_VERTEX10] =   "FIFO_VERTEX10";
	commands[FIFO_VERTEX16] =   "FIFO_VERTEX16";
	commands[FIFO_VERTEX_XZ] =  "FIFO_VERTEX_XZ";
	commands[FIFO_VERTEX_YZ] =  "FIFO_VERTEX_YZ";
	commands[REG2ID(GFX_VERTEX_DIFF)] = "GFX_VERTEX_DIFF";

	commands[FIFO_TEX_COORD] =  "FIFO_TEX_COORD";
	commands[FIFO_TEX_FORMAT] = "FIFO_TEX_FORMAT";
	commands[FIFO_PAL_FORMAT] = "FIFO_PAL_FORMAT";

	commands[FIFO_LIGHT_VECTOR] = "FIFO_LIGHT_VECTOR";
	commands[FIFO_LIGHT_COLOR] = "FIFO_LIGHT_COLOR";
	commands[FIFO_NORMAL] = "FIFO_NORMAL";

	commands[FIFO_BEGIN] =    "FIFO_BEGIN";
	commands[FIFO_END] =      "FIFO_END";
	commands[FIFO_FLUSH] =    "FIFO_FLUSH";
	commands[FIFO_VIEWPORT] = "FIFO_VIEWPORT";

	FILE* file;
	const char* ReadFileName = "teapot.bin";
	fopen_s(&file, ReadFileName, "rb");

	// get the length to allocate a buffer of the given size
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);

	DataType* content = AllocBuffer<DataType>(size);
	fread_s(content, size * sizeof(DataType), sizeof(DataType), size, file);

	int limit = 400;
	if (size > limit)
	{
		size = limit;
	}

	for (int i = 0; i < size; i++)
	{
		DataType data = static_cast<DataType>(content[i]);
		if (commands.find(data) != commands.end())
		{
			std::cout << commands[data] << " ";
		}
		else
		{
			std::cout << std::setfill('0') << std::setw(2) << std::hex << ((int)content[i] & 0x000000FF) << " ";
		}
		if (((i+1) % 4) == 0)
		{
			std::cout << "\n";
		}
	}


	FreeBuffer(content);
	fclose(file);

	return 0;
}
#include <iostream>

#include "InputArguments.h"


int main(int argc, char** argv)
{
    InputArguments inputArguments(argc, argv);
    std::cout << inputArguments.SourceFilePath << std::endl;
    std::cout << inputArguments.OutDirectory << std::endl;
    std::cout << inputArguments.OutFileName << std::endl;
    return 0;
}
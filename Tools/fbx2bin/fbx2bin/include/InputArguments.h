#pragma once

#include <string>

struct InputArguments
{
    InputArguments(int argc, char** argv);

    std::string SourceFilePath = "";
    std::string OutDirectory = "";
    std::string OutFileName = "";
};
#pragma once

#include <string>

struct InputArguments
{
    InputArguments(int argc, char** argv);

    std::string SourceFilePath = "";
    std::string OutDirectory = "";
    std::string OutFileName = "";

    friend std::ostream& operator<<(std::ostream& os, const InputArguments& rhs);

    bool IsValid() const;
};
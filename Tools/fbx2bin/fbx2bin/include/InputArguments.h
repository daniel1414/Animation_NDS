#pragma once

#include <string>

struct InputArguments
{
    InputArguments(int argc, char** argv);

    std::string SourceFilePath = "";
    std::string OutDirectory = "";
    std::string OutFileName = "";
    std::string OutAnimationFile = "";

    friend std::ostream& operator<<(std::ostream& os, const InputArguments& rhs);

    bool IsValid() const;
};
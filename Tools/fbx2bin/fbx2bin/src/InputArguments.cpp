#include "InputArguments.h"

static const std::string filepath = "-filepath=";
static const std::string directory = "-outdir=";
static const std::string filename = "-o=";

InputArguments::InputArguments(int argc, char** argv)
{
    for (int i = 0; i < argc; i++)
    {
        const std::string argument = argv[i];

        if (argument.rfind(filepath, 0) == 0)
        {
            int index = filepath.length();
            SourceFilePath = argument.substr(filepath.length(), argument.length() - filepath.length());
        }
        if (argument.rfind(directory, 0) == 0)
        {
            int index = directory.length();
            OutDirectory = argument.substr(directory.length(), argument.length() - directory.length());
        }
        if (argument.rfind(filename, 0) == 0)
        {
            int index = filename.length();
            OutFileName = argument.substr(filename.length(), argument.length() - filename.length());
        }
    }
}
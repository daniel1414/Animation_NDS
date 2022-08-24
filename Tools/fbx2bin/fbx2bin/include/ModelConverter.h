#pragma once

#include "Mesh.h"

class ModelConverter
{
    Model& m;
public:

    ModelConverter(Model& m) : m(m) {}

    uint32_t* ToNintendoInstructions(uint32_t& OutSize);
};
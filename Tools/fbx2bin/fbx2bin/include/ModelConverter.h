#pragma once

#include "Mesh.h"

class ModelConverter
{
    Model& model;
public:

    ModelConverter(Model& model) : model(model) {}

    uint32_t* ToNintendoInstructions(uint32_t& OutSize);

private:

    uint32_t* ConvertStaticModel(uint32_t& OutSize);

    uint32_t* ConvertAnimatedModel(uint32_t& OutSize);
};
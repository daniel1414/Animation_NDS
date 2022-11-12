#pragma once

#include "Mesh.h"

class ModelConverter
{
    Model& model;
public:

    ModelConverter(Model& model) : model(model) {}

    uint32_t* GetNintendoMeshes(uint32_t& OutSize);

    uint32_t* GetAnimationData(uint32_t& OutSize);

private:
};
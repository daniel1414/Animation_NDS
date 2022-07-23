#pragma once

#include <nds/arm9/videoGL.h>

class Texture
{
public:
    Texture(GL_TEXTURE_TYPE_ENUM type, int sizeX, int sizeY, int param, const void* texture);

    void SetParameter(int param);
    void SetPalette(uint16 width, uint16* palette);

    void Bind();
    static void Bind(int id);

    int GetID() const;
private:
    int m_id;
    static int boundId;
};
#include "Texture.h"

int Texture::boundId = 0;

Texture::Texture(GL_TEXTURE_TYPE_ENUM type, int sizeX, int sizeY, int param, const void* texture)
{
    glGenTextures(1, &m_id);
    Bind();
    glTexImage2D(0, 0, type, sizeX, sizeY, 0, param, texture);
}

void Texture::SetParameter(int param)
{
    Bind();
    glTexParameter(0, param);
}

void Texture::SetPalette(uint16 width, uint16* palette)
{
    Bind();
    glColorTableEXT(0, 0, width, 0, 0, palette);
}

void Texture::Bind()
{
    if(boundId != m_id)
    {
        glBindTexture(0, m_id);
        boundId = m_id;
    }
}
void Texture::Bind(int id)
{
    if(boundId != id)
    {
        glBindTexture(0, id);
        boundId = id;
    }
}

int Texture::GetID() const
{
    return m_id;
}
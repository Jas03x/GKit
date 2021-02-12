#ifndef GK_VERTEX_ARRAY_H
#define GK_VERTEX_ARRAY_H

#include <gk/graphics/graphics.hpp>

class VertexArray
{
private:
    GFX_HANDLE m_Handle;

public:
    VertexArray();
    ~VertexArray();

    GFX_HANDLE GetHandle();

    void Bind();
    void EnableVertexAttribute(unsigned int index);
    void DisableVertexAttribute(unsigned int index);
    void SetVertexAttributeLayoutI(unsigned int index, unsigned int size, unsigned int type, uint32_t stride, const void* offset);
    void SetVertexAttributeLayoutF(unsigned int index, unsigned int size, unsigned int type, uint8_t normalized, uint32_t stride, const void* offset);
};

#endif // GK_VERTEX_ARRAY_H
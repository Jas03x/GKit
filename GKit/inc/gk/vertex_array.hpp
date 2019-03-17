#ifndef GK_VERTEX_ARRAY_H
#define GK_VERTEX_ARRAY_H

#include <gk/graphics.hpp>

class VertexArray
{
private:
    GFX_HANDLE m_Handle;

public:
    VertexArray();
    ~VertexArray();

    GFX_HANDLE GetHandle();

    void Bind();
    void EnableVertexAttribute(uint index);
    void DisableVertexAttribute(uint index);
    void SetVertexAttributeLayout(uint index, uint size, uint type, uint32_t stride, const void* offset);
    void SetVertexAttributeLayout(uint index, uint size, uint type, uint8_t normalized, uint32_t stride, const void* offset);
};

#endif // GK_VERTEX_ARRAY_H
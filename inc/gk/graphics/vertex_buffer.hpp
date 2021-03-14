#ifndef GK_VERTEX_BUFFER_HPP
#define GK_VERTEX_BUFFER_HPP

#include <gk/context/data_types.hpp>
#include <gk/graphics/graphics.hpp>

class VertexBuffer
{
private:
	GFX_HANDLE m_Handle;

    unsigned int m_Type;

public:
    VertexBuffer(unsigned int type);
    ~VertexBuffer();

	GFX_HANDLE GetHandle();

    void Bind();
    void Allocate(int64_t size, const void* data, uint32_t usage);
    void Update(int64_t offset, int64_t size, const void* data);
};

#endif // GK_VERTEX_BUFFER_HPP

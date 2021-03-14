#include <gk/graphics/vertex_buffer.hpp>

#include <cassert>
#include <cstdio>

VertexBuffer::VertexBuffer(unsigned int type)
{
    const RenderingContext* context = RenderingContext::GetInstance();
    context->CreateAllocations(1, &m_Handle);

    switch(type)
    {
        case GFX_ARRAY_BUFFER:
        case GFX_ELEMENT_BUFFER:
        {
            m_Type = type;
            break;
        }
        
        default:
        {
            printf("Error: Unknown buffer type passed to vertex buffer\n");
            break;
        }
    }
}

VertexBuffer::~VertexBuffer()
{
    const RenderingContext* context = RenderingContext::GetInstance();
    if(context->IsAllocation(m_Handle) == GFX_TRUE)
    {
        context->DeleteAllocations(1, &m_Handle);
    }
}

GFX_HANDLE VertexBuffer::GetHandle()
{
    return m_Handle;
}

void VertexBuffer::Bind()
{
    RenderingContext::GetInstance()->BindAllocation(m_Type, m_Handle);
}

void VertexBuffer::Allocate(int64_t size, const void* data, uint32_t usage)
{
    const RenderingContext* context = RenderingContext::GetInstance();
    context->CreateBuffer(m_Type, size, data, usage);
}

void VertexBuffer::Update(int64_t offset, int64_t size, const void* data)
{
    const RenderingContext* context = RenderingContext::GetInstance();
    context->UpdateBuffer(m_Type, offset, size, data);
}


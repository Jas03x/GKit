#include <gk/vertex_buffer.hpp>

#include <stdio.h>

#include <gk/assert.hpp>

VertexBuffer::VertexBuffer(uint type)
{
    RenderingContext* context = RenderingContext::GetInstance();
    context->CreateAllocations(1, &m_Handle);

    switch(type)
    {
        case GFX_ARRAY_BUFFER:
        case GFX_ELEMENT_BUFFER:
            m_Type = type;
            break;
        default:
            GK_ASSERT(false, ("ERROR: Unknown buffer type passed to vertex buffer\n"));
    }
}

VertexBuffer::~VertexBuffer()
{
    RenderingContext* context = RenderingContext::GetInstance();
    if(context->IsAllocation(m_Handle) == true)
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
    RenderingContext* context = RenderingContext::GetInstance();
    context->BindAllocation(m_Type, m_Handle);
    context->CreateBuffer(m_Type, size, data, usage);
}

void VertexBuffer::Update(int64_t offset, int64_t size, const void* data)
{
    RenderingContext* context = RenderingContext::GetInstance();
    context->BindAllocation(m_Type, m_Handle);
    context->UpdateBuffer(m_Type, offset, size, data);
}


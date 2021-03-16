#include <gk/core/graphics/vertex_array.hpp>

VertexArray::VertexArray()
{
    const RenderingContext* context = RenderingContext::GetInstance();
    context->CreateVertexArrays(1, &m_Handle);
}

VertexArray::~VertexArray()
{
    const RenderingContext* context = RenderingContext::GetInstance();
    if(context->IsVertexArray(m_Handle) == GFX_TRUE)
    {
        context->DeleteVertexArrays(1, &m_Handle);
    }
}

GFX_HANDLE VertexArray::GetHandle()
{
    return m_Handle;
}

void VertexArray::Bind()
{
    RenderingContext::GetInstance()->BindVertexArray(m_Handle);
}

void VertexArray::EnableVertexAttribute(unsigned int index)
{
    RenderingContext::GetInstance()->EnableVertexAttribute(index);
}

void VertexArray::DisableVertexAttribute(unsigned int index)
{
    RenderingContext::GetInstance()->DisableVertexAttribute(index);
}

void VertexArray::SetVertexAttributeLayoutI(unsigned int index, unsigned int size, unsigned int type, uint32_t stride, const void* offset)
{
    RenderingContext::GetInstance()->SetVertexAttributeLayoutI(index, size, type, stride, offset);
}

void VertexArray::SetVertexAttributeLayoutF(unsigned int index, unsigned int size, unsigned int type, uint8_t normalized, uint32_t stride, const void* offset)
{
    RenderingContext::GetInstance()->SetVertexAttributeLayoutF(index, size, type, normalized, stride, offset);
}

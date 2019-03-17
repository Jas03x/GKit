#include <gk/vertex_array.hpp>

VertexArray::VertexArray()
{
    RenderingContext* context = RenderingContext::GetInstance();
    context->CreateVertexArrays(1, &m_Handle);
}

VertexArray::~VertexArray()
{
    RenderingContext* context = RenderingContext::GetInstance();
    if(context->IsVertexArray(m_Handle) == true)
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

void VertexArray::EnableVertexAttribute(uint index)
{
    RenderingContext::GetInstance()->EnableVertexAttribute(index);
}

void VertexArray::DisableVertexAttribute(uint index)
{
    RenderingContext::GetInstance()->DisableVertexAttribute(index);
}

void VertexArray::SetVertexAttributeLayout(uint index, uint size, uint type, uint32_t stride, const void* offset)
{
    RenderingContext::GetInstance()->SetVertexAttributeLayoutI(index, size, type, stride, offset);
}

void VertexArray::SetVertexAttributeLayout(uint index, uint size, uint type, uint8_t normalized, uint32_t stride, const void* offset)
{
    RenderingContext::GetInstance()->SetVertexAttributeLayoutF(index, size, type, normalized, stride, offset);
}

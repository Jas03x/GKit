#include <gk/3d/graphics/render_buffer.hpp>

RenderBuffer::RenderBuffer(unsigned int width, unsigned int height, unsigned int format)
{
    const RenderingContext* context = RenderingContext::GetInstance();
    context->CreateRenderbuffers(1, &m_Handle);
    context->BindRenderbuffer(GFX_RENDERBUFFER, m_Handle);
    context->RenderbufferStorage(GFX_RENDERBUFFER, format, width, height);
}

RenderBuffer::~RenderBuffer()
{
    const RenderingContext* context = RenderingContext::GetInstance();

    if (context->IsRenderbuffer(m_Handle) == GFX_TRUE)
    {
        context->DeleteRenderbuffers(1, &m_Handle);
    }
}

GFX_HANDLE RenderBuffer::GetHandle()
{
    return m_Handle;
}

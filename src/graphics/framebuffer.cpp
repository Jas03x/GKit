#include <gk/framebuffer.hpp>

#include <cassert>
#include <cstdio>

#include <memory>

Framebuffer::Framebuffer(unsigned int width, unsigned int height)
{
    m_Width  = width;
    m_Height = height;
    const RenderingContext* context = RenderingContext::GetInstance();

    context->CreateFramebuffers(1, &m_Handle);
}

void Framebuffer::Attach(CubeMap* attachment, GFX_HANDLE face, GFX_HANDLE target)
{
    const RenderingContext* context = RenderingContext::GetInstance();
    context->SetFramebufferTexture2D(GFX_FRAMEBUFFER, target, face, attachment->GetHandle(), 0);
}

void Framebuffer::Attach(Texture* attachment, GFX_HANDLE target)
{
    const RenderingContext* context = RenderingContext::GetInstance();
    context->SetFramebufferTexture2D(GFX_FRAMEBUFFER, target, GFX_TEXTURE_2D, attachment->GetHandle(), 0);
}

void Framebuffer::Attach(RenderBuffer* attachment, GFX_HANDLE target)
{
    const RenderingContext* context = RenderingContext::GetInstance();
    context->SetFramebufferRenderbuffer(GFX_FRAMEBUFFER, target, GFX_RENDERBUFFER, attachment->GetHandle());
}

bool Framebuffer::IsValid()
{
    const RenderingContext* context = RenderingContext::GetInstance();
    return context->GetFramebufferStatus(GFX_FRAMEBUFFER) == GFX_COMPLETE_FRAMEBUFFER;
}

Framebuffer::~Framebuffer()
{
    const RenderingContext* context = RenderingContext::GetInstance();
    if((m_Handle != 0) && (context->IsFramebuffer(m_Handle) == GFX_TRUE))
    {
        context->DeleteFramebuffers(1, &m_Handle);
    }
}

void Framebuffer::Bind() const
{
    const RenderingContext* context = RenderingContext::GetInstance();

    context->BindFramebuffer(GFX_FRAMEBUFFER, m_Handle);
    context->SetViewport(0, 0, m_Width, m_Height);
}

unsigned int Framebuffer::GetWidth() const
{
    return m_Width;
}

unsigned int Framebuffer::GetHeight() const
{
    return m_Height;
}

/************************************************************************************************************************************/
/************************* Default framebuffer interface ****************************************************************************/
/************************************************************************************************************************************/

static Framebuffer* DefaultFramebuffer;

void Framebuffer::InitializeDefaultFramebuffer(unsigned int width, unsigned int height)
{
    // we use the standard allocator because we do not want to use any constructor -- we want to simply assign the fields ourselves
    std::allocator<Framebuffer> allocator;
    DefaultFramebuffer = allocator.allocate(1);

    DefaultFramebuffer->m_Handle = 0;
    DefaultFramebuffer->m_Width  = width;
    DefaultFramebuffer->m_Height = height;
}

void Framebuffer::DestroyDefaultFramebuffer()
{
    delete DefaultFramebuffer;
}

const Framebuffer* Framebuffer::GetDefaultFramebuffer()
{
    return DefaultFramebuffer;
}

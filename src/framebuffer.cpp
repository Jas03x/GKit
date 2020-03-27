#include <gk/framebuffer.hpp>

#include <assert.h>
#include <stdio.h>

#include <memory>

Framebuffer::Attachment::Attachment(unsigned int width, unsigned int height, unsigned int format)
{
    m_Type = BUFFER;

    const RenderingContext* context = RenderingContext::GetInstance();
    context->CreateRenderbuffers(1, &m_Handle);
    context->BindRenderbuffer(GFX_RENDERBUFFER, m_Handle);
    context->RenderbufferStorage(GFX_RENDERBUFFER, format, width, height);
}

Framebuffer::Attachment::Attachment(unsigned int width, unsigned int height, unsigned int component_format, unsigned int component_type, unsigned int filter, unsigned int wrap_mode)
{
    m_Type = TEXTURE;

    const RenderingContext* context = RenderingContext::GetInstance();
    context->CreateTextures(1, &m_Handle);
    context->BindTexture(GFX_TEXTURE_2D, m_Handle);
    context->CreateTexture2D(GFX_TEXTURE_2D, 0, component_format, width, height, 0, component_format, component_type, NULL);
    
    context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_MAG_FILTER, filter);
    context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_MIN_FILTER, filter);
    context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_WRAP_S, wrap_mode);
	context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_WRAP_T, wrap_mode);
}

Framebuffer::Attachment::~Attachment()
{
    const RenderingContext* context = RenderingContext::GetInstance();

    switch(m_Type)
    {
        case BUFFER:
        {
            if(context->IsRenderbuffer(m_Handle) == GFX_TRUE)
            {
                context->DeleteRenderbuffers(1, &m_Handle);
            }
            break;
        }

        case TEXTURE:
        {
            if(context->IsTexture(m_Handle) == GFX_TRUE)
            {
                context->DeleteTextures(1, &m_Handle);
            }
            break;
        }

        default:
        {
            printf("ERROR: Invalid framebuffer attachment type\n");
            break;
        }
    }
}

Framebuffer::Attachment::ATTACHMENT_TYPE Framebuffer::Attachment::GetType()
{
    return m_Type;
}

GFX_HANDLE Framebuffer::Attachment::GetHandle()
{
    return m_Handle;
}

void Framebuffer::Attachment::Bind(GFX_HANDLE target, GFX_HANDLE attachment)
{
    const RenderingContext* context = RenderingContext::GetInstance();

    switch(m_Type)
    {
        case BUFFER:
        {
            context->SetFramebufferRenderbuffer(target, attachment, GFX_RENDERBUFFER, m_Handle);
            break;
        }

        case TEXTURE:
        {
            context->SetFramebufferTexture2D(target, attachment, GFX_TEXTURE_2D, m_Handle, 0);
            break;
        }

        default:
        {
            printf("Framebuffer invalid storage type\n");
            break;
        }
    }
}

Framebuffer::Framebuffer(unsigned int width, unsigned height, Attachment* colorAttachment, Attachment* depthAttachment)
{
    m_Width  = width;
    m_Height = height;
    const RenderingContext* context = RenderingContext::GetInstance();

    context->CreateFramebuffers(1, &m_Handle);
    context->BindFramebuffer(GFX_FRAMEBUFFER, m_Handle);

    depthAttachment->Bind(GFX_FRAMEBUFFER, GFX_DEPTH_ATTACHMENT);
    colorAttachment->Bind(GFX_FRAMEBUFFER, GFX_COLOR_ATTACHMENT0);
    
    GFX_HANDLE DrawBuffers[1] = { GFX_COLOR_ATTACHMENT0 };
    context->BindDrawBuffers(1, DrawBuffers);

    assert(context->GetFramebufferStatus(GFX_FRAMEBUFFER) == GFX_COMPLETE_FRAMEBUFFER);

    context->BindFramebuffer(GFX_FRAMEBUFFER, 0);
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

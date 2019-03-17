#ifndef GK_FRAMEBUFFER_H
#define GK_FRAMEBUFFER_H

#include <gk/graphics.hpp>

class Framebuffer
{
// General framebuffer interface:
public:
    class Attachment
    {
    public:
        typedef enum
        {
            BUFFER  = 0,
            TEXTURE = 1
        } ATTACHMENT_TYPE;

    private:
		GFX_HANDLE m_Handle;
        ATTACHMENT_TYPE m_Type;

    public:
        Attachment(uint width, uint height, uint format); // creates a RENDER-BUFFER
        Attachment(uint width, uint height, uint component_format, uint component_type, uint filter, uint wrap_mode); // creates a RENDER-TEXTURE
        ~Attachment();

        void Bind(uint target, uint attachment);

		GFX_HANDLE GetHandle();
        ATTACHMENT_TYPE GetType();
    };

private:
	GFX_HANDLE m_Handle;

    uint m_Width;
    uint m_Height;

public:
    Framebuffer(uint width, uint height, Attachment* colorAttachment, Attachment* depthAttachment);
    ~Framebuffer();

    void Bind() const;
    uint GetWidth() const;
    uint GetHeight() const;

    static void InitializeDefaultFramebuffer(uint width, uint height);
    static void DestroyDefaultFramebuffer();
    static const Framebuffer* GetDefaultFramebuffer();
};

#endif // GK_FRAMEBUFFER_H
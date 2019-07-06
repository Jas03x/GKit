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
        Attachment(unsigned int width, unsigned int height, unsigned int format); // creates a RENDER-BUFFER
        Attachment(unsigned int width, unsigned int height, unsigned int component_format, unsigned int component_type, unsigned int filter, unsigned int wrap_mode); // creates a RENDER-TEXTURE
        ~Attachment();

        void Bind(unsigned int target, unsigned int attachment);

		GFX_HANDLE GetHandle();
        ATTACHMENT_TYPE GetType();
    };

private:
	GFX_HANDLE m_Handle;

    unsigned int m_Width;
    unsigned int m_Height;

public:
    Framebuffer(unsigned int width, unsigned int height, Attachment* colorAttachment, Attachment* depthAttachment);
    ~Framebuffer();

    void Bind() const;
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    static void InitializeDefaultFramebuffer(unsigned int width, unsigned int height);
    static void DestroyDefaultFramebuffer();
    static const Framebuffer* GetDefaultFramebuffer();
};

#endif // GK_FRAMEBUFFER_H
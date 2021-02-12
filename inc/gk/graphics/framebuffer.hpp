#ifndef GK_FRAMEBUFFER_H
#define GK_FRAMEBUFFER_H

#include <gk/graphics/graphics.hpp>
#include <gk/graphics/cubemap.hpp>
#include <gk/graphics/render_buffer.hpp>
#include <gk/graphics/texture.hpp>

class Framebuffer
{
private:
	GFX_HANDLE m_Handle;

    unsigned int m_Width;
    unsigned int m_Height;

public:
    Framebuffer(unsigned int width, unsigned int height);
    ~Framebuffer();

    bool IsValid();

    void Attach(Texture* attachment, GFX_HANDLE target);
    void Attach(RenderBuffer* attachment, GFX_HANDLE target);
    void Attach(CubeMap* attachment, GFX_HANDLE face, GFX_HANDLE target);

    void Bind() const;
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    static void InitializeDefaultFramebuffer(unsigned int width, unsigned int height);
    static void DestroyDefaultFramebuffer();
    static const Framebuffer* GetDefaultFramebuffer();
};

#endif // GK_FRAMEBUFFER_H

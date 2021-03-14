#ifndef GK_RENDER_BUFFER_HPP
#define GK_RENDER_BUFFER_HPP

#include <gk/graphics/graphics.hpp>

class RenderBuffer
{
private:
	GFX_HANDLE m_Handle;

public:
	RenderBuffer(unsigned int width, unsigned int height, unsigned int format);
	~RenderBuffer();

	GFX_HANDLE GetHandle();
};

#endif // GK_RENDER_BUFFER_HPP

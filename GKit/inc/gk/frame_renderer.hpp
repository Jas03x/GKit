#ifndef GK_FRAME_RENDERER_H
#define GK_FRAME_RENDERER_H

#include <gk/shader.hpp>

class FrameRenderer : public Shader
{
private:
    static FrameRenderer* m_Instance;

	GFX_HANDLE m_TextureID;
	GFX_HANDLE m_Opacity;

private:
    FrameRenderer();
    ~FrameRenderer();

public:
    static void CreateInstance();
    static void DeleteInstance();

    static void Bind();
    static void Render(GFX_HANDLE textureID, float opacity = 1.0f);
};

#endif // GK_FRAME_RENDERER
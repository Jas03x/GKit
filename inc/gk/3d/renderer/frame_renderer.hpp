#ifndef GK_FRAME_RENDERER_H
#define GK_FRAME_RENDERER_H

#include <gk/3d/graphics/shader.hpp>

class FrameRenderer : public Shader
{
private:
    static FrameRenderer* Instance;

private:
	GFX_HANDLE m_TextureID;
	GFX_HANDLE m_Opacity;

private:
    FrameRenderer();
    ~FrameRenderer();

public:
    static FrameRenderer* CreateInstance();
    static void DeleteInstance();
    
    static FrameRenderer* GetInstance();

public:
    void Bind();
    void Render(GFX_HANDLE textureID, float opacity = 1.0f);
};

#endif // GK_FRAME_RENDERER

#ifndef GK_BLUR_SHADER_H
#define GK_BLUR_SHADER_H

#include <gk/graphics/shader.hpp>

class BlurShader : public Shader
{
private:
    static BlurShader* Instance;

private:
	GFX_HANDLE m_TextureID;
	GFX_HANDLE m_Length;
	GFX_HANDLE m_Direction;

private:
    BlurShader();
    ~BlurShader();

public:
    static BlurShader* CreateInstance();
    static void DeleteInstance();
    
    static BlurShader* GetInstance();

public:
    void Bind();
    void VerticalBlur(GFX_HANDLE textureID, float height);
    void HorizontalBlur(GFX_HANDLE textureID, float width);
};

#endif // GK_BLUR_SHADER_H

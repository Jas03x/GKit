#ifndef GK_BLUR_SHADER_H
#define GK_BLUR_SHADER_H

#include <gk/shader.hpp>

class BlurShader : public Shader
{
private:
    static BlurShader* m_Instance;

	GFX_HANDLE m_TextureID;
	GFX_HANDLE m_Length;
	GFX_HANDLE m_Direction;

private:
    BlurShader();
    ~BlurShader();

public:
    static void CreateInstance();
    static void DeleteInstance();

    static void Bind();
    static void VerticalBlur(GFX_HANDLE textureID, float height);
    static void HorizontalBlur(GFX_HANDLE textureID, float width);
};

#endif // GK_BLUR_SHADER_H
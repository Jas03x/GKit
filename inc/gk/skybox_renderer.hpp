#ifndef GK_SKYBOX_RENDERER_H
#define GK_SKYBOX_RENDERER_H

#include <gk/shader.hpp>
#include <gk/skybox.hpp>

class SkyboxRenderer : public Shader
{
private:
	static SkyboxRenderer* Instance;

private:
	GFX_HANDLE m_CubeMap;
	GFX_HANDLE m_InverseViewProjectionMatrix;

private:
	SkyboxRenderer();
	~SkyboxRenderer();

public:
	static SkyboxRenderer* CreateInstance();
	static void DeleteInstance();
    
    static SkyboxRenderer* GetInstance();

public:
	void Bind();
	void Render(const Skybox& skybox);
};

#endif // GK_SKYBOX_RENDERER_H

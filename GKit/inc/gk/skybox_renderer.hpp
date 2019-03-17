#ifndef GK_SKYBOX_RENDERER_H
#define GK_SKYBOX_RENDERER_H

#include <gk/shader.hpp>
#include <gk/skybox.hpp>

class SkyboxRenderer : public Shader
{
private:
	static SkyboxRenderer* Instance;

	GFX_HANDLE m_CubeMap;
	GFX_HANDLE m_InverseViewProjectionMatrix;

private:
	SkyboxRenderer();
	~SkyboxRenderer();

public:
	static void CreateInstance();
	static void DeleteInstance();

	static void Bind();
	static void Render(const Skybox& skybox);
};

#endif // GK_SKYBOX_RENDERER_H

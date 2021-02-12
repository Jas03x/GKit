#ifndef GK_PARTICLE_RENDERER_H
#define GK_PARTICLE_RENDERER_H

#include <gk/graphics/graphics.hpp>
#include <gk/graphics/shader.hpp>
#include <gk/graphics/particle_array.hpp>

class ParticleRenderer3D : public Shader
{
private:
	static ParticleRenderer3D* Instance;

private:
	unsigned int m_SpriteTexture;
	unsigned int m_SpriteSize;
	unsigned int m_ViewMatrix;
	unsigned int m_ProjectionMatrix;

private:
	ParticleRenderer3D();
	~ParticleRenderer3D();

public:
	static ParticleRenderer3D* CreateInstance();
	static void DeleteInstance();
    
    static ParticleRenderer3D* GetInstanace();
    
public:
	void Bind();
	void Render(const ParticleArray& particle_array);
};

#endif // GK_PARTICLE_RENDERER_H

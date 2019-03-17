#ifndef GK_PARTICLE_RENDERER_H
#define GK_PARTICLE_RENDERER_H

#include <gk/graphics.hpp>
#include <gk/shader.hpp>
#include <gk/particle_array.hpp>

class ParticleRenderer3D : public Shader
{
private:
	static ParticleRenderer3D* Instance;

	uint m_Matrix;
	uint m_SpriteTexture;
	uint m_SpriteSize;

private:
	ParticleRenderer3D();
	~ParticleRenderer3D();

public:
	static void CreateInstance();
	static void DeleteInstance();

	static void Bind();
	static void Render(const ParticleArray& particle_array);
};

#endif // GK_PARTICLE_RENDERER_H
#ifndef GK_PARTICLE_ARRAY_H
#define GK_PARTICLE_ARRAY_H

#include <vector>

#include <gk/core/context/data_types.hpp>
#include <gk/core/math/linear.hpp>
#include <gk/core/graphics/graphics.hpp>
#include <gk/core/graphics/texture.hpp>
#include <gk/core/graphics/particle.hpp>

class ParticleArray
{
private:
	GFX_HANDLE m_VAO;
	GFX_HANDLE m_VBO;

    Texture* m_Texture;
    Vector2F m_SpriteSize;

    unsigned int m_ParticleLimit; // maximum number of particles
	unsigned int m_ParticleCount; // number of particles in the buffer

protected:
    Particle* m_Particles;

    void UpdateBuffer(unsigned int particle_count);

public:
    typedef enum
    {
        POSITION = 0,
        UV_COORD = 1,
        SIZE     = 2
    } VertexAttributes;

    typedef struct
    {
        float			position[3];
        unsigned char	uv_coord[2];
		unsigned short	size;
    } VertexFormat;

    ParticleArray(const char* texture_sheet, const Vector2F& dimensions, unsigned int particle_limit);
    virtual ~ParticleArray();

    const Texture* GetTexture() const;
    const Vector2F& GetSpriteSize() const;
    unsigned int GetParticleCount() const;
    unsigned int GetParticleLimit() const;

    virtual void Update() = 0;

    void Bind() const;
};

#endif // GK_PARTICLE_ARRAY_H

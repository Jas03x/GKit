#include <gk/particle_array.hpp>

#include <gk/tga_image.hpp>

ParticleArray::ParticleArray(const char* texture_sheet, const Vector2F& dimensions, unsigned int particle_limit)
{
    TgaImage image(texture_sheet);
    m_Texture = new Texture(image.HasAlpha() ? GFX_RGBA : GFX_RGB, image.GetWidth(), image.GetHeight(), GFX_TYPE_UNSIGNED_BYTE, image.GetPixels(), GFX_LINEAR, GFX_CLAMP_TO_EDGE);

    m_SpriteSize = dimensions;
    m_ParticleCount = 0;
    m_ParticleLimit = particle_limit;

    m_Particles = new Particle[particle_limit];

    RenderingContext* context = RenderingContext::GetInstance();

    context->CreateVertexArrays(1, &m_VAO);
    context->BindVertexArray(m_VAO);

    context->CreateAllocations(1, &m_VBO);
    context->BindAllocation(GFX_ARRAY_BUFFER, m_VBO);
    context->CreateBuffer(GFX_ARRAY_BUFFER, sizeof(Particle) * m_ParticleLimit, NULL, GFX_DYNAMIC_DRAW);

    context->EnableVertexAttribute(VertexAttributes::POSITION);
    context->EnableVertexAttribute(VertexAttributes::UV_COORD);
    context->EnableVertexAttribute(VertexAttributes::SIZE);
    context->SetVertexAttributeLayoutF(VertexAttributes::POSITION,  3, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(VertexFormat), (void*) offsetof(VertexFormat, position));
    context->SetVertexAttributeLayoutI(VertexAttributes::UV_COORD,  2, GFX_TYPE_UNSIGNED_BYTE,	  sizeof(VertexFormat), (void*) offsetof(VertexFormat, uv_coord));
    context->SetVertexAttributeLayoutI(VertexAttributes::SIZE,      1, GFX_TYPE_UNSIGNED_SHORT,	  sizeof(VertexFormat), (void*) offsetof(VertexFormat, size));

    context->BindVertexArray(0);
}

ParticleArray::~ParticleArray()
{
    RenderingContext* context = RenderingContext::GetInstance();

    delete m_Texture;
    delete m_Particles;

    if(context->IsVertexArray(m_VAO) == true) {
        context->DeleteVertexArrays(1, &m_VAO);
    }
    
    if(context->IsAllocation(m_VBO) == true) {
        context->DeleteAllocations(1, &m_VBO);
    }
}

const Texture* ParticleArray::GetTexture() const {
    return m_Texture;
}

const Vector2F& ParticleArray::GetSpriteSize() const {
    return m_SpriteSize;
}

unsigned int ParticleArray::GetParticleCount() const {
    return m_ParticleCount;
}

unsigned int ParticleArray::GetParticleLimit() const {
    return m_ParticleLimit;
}

void ParticleArray::UpdateBuffer()
{
    RenderingContext* context = RenderingContext::GetInstance();

    context->BindAllocation(GFX_ARRAY_BUFFER, m_VBO);
    context->CreateBuffer(GFX_ARRAY_BUFFER,    sizeof(Particle) * m_ParticleLimit, NULL, GFX_DYNAMIC_DRAW);
    context->UpdateBuffer(GFX_ARRAY_BUFFER, 0, sizeof(Particle) * m_ParticleCount, m_Particles);
}

void ParticleArray::Bind() const
{
    RenderingContext::GetInstance()->BindVertexArray(m_VAO);
}

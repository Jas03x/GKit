#include <gk/particle_renderer_3d.hpp>

#include <assert.h>

#include <gk/file.hpp>
#include <gk/view.hpp>
#include <gk/camera_3d.hpp>

ParticleRenderer3D* ParticleRenderer3D::Instance = nullptr;

ParticleRenderer3D::ParticleRenderer3D()
{
    Shader::Load(
		File::Read(SHADER_BASE "particle.vert").c_str(), 
		File::Read(SHADER_BASE "particle.frag").c_str(), 
		[](GFX_HANDLE id) {
			RenderingContext* context = RenderingContext::GetInstance();
			context->SetAttributeLocation(id, ParticleArray::VertexAttributes::POSITION, "position");
			context->SetAttributeLocation(id, ParticleArray::VertexAttributes::UV_COORD, "uv_coord");
			context->SetAttributeLocation(id, ParticleArray::VertexAttributes::SIZE, 	 "size");
		}
	);

	m_Matrix			= GetUniformLocation("matrix");
	m_SpriteTexture		= GetUniformLocation("sprite");
	m_SpriteSize		= GetUniformLocation("sprite_size");
}

ParticleRenderer3D::~ParticleRenderer3D()
{

}

void ParticleRenderer3D::CreateInstance()
{
	assert(Instance == nullptr);
    if (Instance == nullptr)
	{
		Instance = new ParticleRenderer3D();
	}
}

void ParticleRenderer3D::DeleteInstance()
{
	assert(Instance != nullptr);
    if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

void ParticleRenderer3D::Bind()
{
	assert(Instance != nullptr);
    Instance->Shader::Bind();
}

void ParticleRenderer3D::Render(const ParticleArray& particle_array)
{
	assert(Instance != nullptr);
	
	RenderingContext* context = RenderingContext::GetInstance();
	Matrix4F matrix = Camera3D::GetInstance()->GetMatrix();

	particle_array.Bind();
	particle_array.GetTexture()->Bind(Instance->m_SpriteTexture, 0);
	context->LoadConstantArray2F(Instance->m_SpriteSize, 1, &particle_array.GetSpriteSize()[0]);
	context->LoadConstantMatrix4F(Instance->m_Matrix, 1, false, &matrix[0][0]);
	context->DrawArray(GFX_POINTS, 0, particle_array.GetParticleCount());
}

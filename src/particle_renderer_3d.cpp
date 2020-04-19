#include <gk/particle_renderer_3d.hpp>

#include <assert.h>

#include <gk/config.hpp>
#include <gk/file.hpp>
#include <gk/view.hpp>
#include <gk/camera_3d.hpp>

#include "../shdr/particle_pixel_shader.hpp"
#include "../shdr/particle_vertex_shader.hpp"

ParticleRenderer3D* ParticleRenderer3D::Instance = nullptr;

ParticleRenderer3D::ParticleRenderer3D()
{
    Shader::Load(
		PARTICLE_VERTEX_SHADER,
		PARTICLE_PIXEL_SHADER,
		[](GFX_HANDLE id) {
			const RenderingContext* context = RenderingContext::GetInstance();
			context->SetAttributeLocation(id, ParticleArray::VertexAttributes::POSITION, "position");
			context->SetAttributeLocation(id, ParticleArray::VertexAttributes::UV_COORD, "uv_coord");
			context->SetAttributeLocation(id, ParticleArray::VertexAttributes::SIZE, 	 "size");
		}
	);

	m_SpriteTexture	   = GetUniformLocation("sprite");
	m_SpriteSize	   = GetUniformLocation("sprite_size");
	m_ViewMatrix	   = GetUniformLocation("view_matrix");
	m_ProjectionMatrix = GetUniformLocation("projection_matrix");
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
	
	const RenderingContext* context = RenderingContext::GetInstance();
	
	Camera3D* camera = Camera3D::GetInstance();
	Matrix4F view_matrix = camera->GetViewMatrix();
	Matrix4F projection_matrix = camera->GetProjectionMatrix();

	particle_array.Bind();
	particle_array.GetTexture()->Bind(Instance->m_SpriteTexture, 0);

	context->LoadConstantArray2F(Instance->m_SpriteSize, 1, &particle_array.GetSpriteSize()[0]);
	context->LoadConstantMatrix4F(Instance->m_ViewMatrix, 1, false, &view_matrix[0][0]);
	context->LoadConstantMatrix4F(Instance->m_ProjectionMatrix, 1, false, &projection_matrix[0][0]);

	context->DrawArray(GFX_POINTS, 0, particle_array.GetParticleCount());
}

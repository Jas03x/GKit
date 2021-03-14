#include <gk/3d/renderer/particle_renderer_3d.hpp>

#include <cassert>

#include <gk/3d/config/config.hpp>
#include <gk/3d/core/view.hpp>
#include <gk/3d/core/camera_3d.hpp>
#include <gk/core/io/file.hpp>

#include "../../shdr/particle_pixel_shader.hpp"
#include "../../shdr/particle_vertex_shader.hpp"

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

ParticleRenderer3D* ParticleRenderer3D::CreateInstance()
{
	assert(Instance == nullptr);
    if (Instance == nullptr)
	{
		Instance = new ParticleRenderer3D();
	}
    return Instance;
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

ParticleRenderer3D* ParticleRenderer3D::GetInstanace()
{
    return Instance;
}

void ParticleRenderer3D::Bind()
{
    Shader::Bind();
}

void ParticleRenderer3D::Render(const ParticleArray& particle_array)
{
	const RenderingContext* context = RenderingContext::GetInstance();
	
	Camera3D* camera = Camera3D::GetInstance();
	Matrix4F view_matrix = camera->GetViewMatrix();
	Matrix4F projection_matrix = camera->GetProjectionMatrix();

	particle_array.Bind();
	particle_array.GetTexture()->Bind(m_SpriteTexture, 0);

	context->LoadConstantArray2F(m_SpriteSize, 1, &particle_array.GetSpriteSize()[0]);
	context->LoadConstantMatrix4F(m_ViewMatrix, 1, false, &view_matrix[0][0]);
	context->LoadConstantMatrix4F(m_ProjectionMatrix, 1, false, &projection_matrix[0][0]);

	context->DrawArray(GFX_POINTS, 0, particle_array.GetParticleCount());
}

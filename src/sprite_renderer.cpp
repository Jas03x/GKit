#include <gk/sprite_renderer.hpp>

#include <assert.h>
#include <stdio.h>

#include <gk/config.hpp>
#include <gk/file.hpp>
#include <gk/quad.hpp>
#include <gk/camera_2d.hpp>
#include <gk/matrix.hpp>

#include "../shdr/sprite_pixel_shader.hpp"
#include "../shdr/sprite_vertex_shader.hpp"

SpriteRenderer* SpriteRenderer::Instance = nullptr;

SpriteRenderer::SpriteRenderer()
{
	Shader::Load(
		SPRITE_VERTEX_SHADER,
		SPRITE_PIXEL_SHADER,
		[](GFX_HANDLE id) {
			RenderingContext::GetInstance()->SetAttributeLocation(id, Quad::VertexAttributes::VERTEX, "vertex");
		}
	);

	m_MatrixLocation = Shader::GetUniformLocation("matrix");
	m_SpriteLocation = Shader::GetUniformLocation("sprite");
}

SpriteRenderer::~SpriteRenderer()
{

}

void SpriteRenderer::CreateInstance()
{
	assert(Instance == nullptr);
	if (Instance == nullptr)
	{
		Instance = new SpriteRenderer();
	}
}

void SpriteRenderer::DeleteInstance()
{
	assert(Instance != nullptr);
	if (Instance)
	{
		delete Instance;
		Instance = NULL;
	}
}

void SpriteRenderer::Bind()
{
	assert(Instance != nullptr);

	Instance->Shader::Bind();
	Quad::Bind();
}

void SpriteRenderer::Render(const Sprite& sprite)
{
	assert(Instance != nullptr);

	const RenderingContext* context = RenderingContext::GetInstance();
	Matrix3F matrix = Camera2D::GetInstance()->GetMatrix() * sprite.Transform.ToMatrix();

	context->LoadConstantMatrix3F(Instance->m_MatrixLocation, 1, GFX_FALSE, &matrix[0][0]);
	sprite.Bind(Instance->m_SpriteLocation, 0);

	context->DrawArray(GFX_TRIANGLES, 0, Quad::GetVertexCount());
}

void SpriteRenderer::Render(const Sprite& sprite, const std::vector<Transform2D>& instances)
{
	assert(Instance != nullptr);

	Matrix3F matrix;
	Matrix3F vp_matrix = Camera2D::GetInstance()->GetMatrix();
	
	const RenderingContext* context = RenderingContext::GetInstance();
	sprite.Bind(Instance->m_SpriteLocation, 0);

	for (unsigned int i = 0; i < instances.size(); i++)
	{
		matrix = vp_matrix * instances[i].ToMatrix();
		context->LoadConstantMatrix3F(Instance->m_MatrixLocation, 1, GFX_FALSE, &matrix[0][0]);

		context->DrawArray(GFX_TRIANGLES, 0, Quad::GetVertexCount());
	}
}

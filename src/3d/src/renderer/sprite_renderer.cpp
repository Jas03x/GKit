#include <gk/3d/renderer/sprite_renderer.hpp>

#include <cassert>
#include <cstdio>

#include <gk/3d/config/config.hpp>
#include <gk/core/io/file.hpp>
#include <gk/core/graphics/quad.hpp>
#include <gk/3d/core/camera_2d.hpp>
#include <gk/core/math/linear.hpp>

#include "../../shdr/sprite_pixel_shader.hpp"
#include "../../shdr/sprite_vertex_shader.hpp"

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

SpriteRenderer* SpriteRenderer::CreateInstance()
{
	assert(Instance == nullptr);
	if (Instance == nullptr)
	{
		Instance = new SpriteRenderer();
	}
    return Instance;
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

SpriteRenderer* SpriteRenderer::GetInstance()
{
    return Instance;
}

void SpriteRenderer::Bind()
{
	Shader::Bind();
	Quad::GetInstance()->Bind();
}

void SpriteRenderer::Render(const Sprite& sprite)
{
	assert(Instance != nullptr);

	const RenderingContext* context = RenderingContext::GetInstance();
	Matrix3F matrix = Camera2D::GetInstance()->GetMatrix() * sprite.Transform.ToMatrix();

	context->LoadConstantMatrix3F(m_MatrixLocation, 1, GFX_FALSE, &matrix[0][0]);
	sprite.Bind(m_SpriteLocation, 0);

	context->DrawArray(GFX_TRIANGLES, 0, Quad::GetInstance()->GetVertexCount());
}

void SpriteRenderer::Render(const Sprite& sprite, const std::vector<Transform2D>& instances)
{
	assert(Instance != nullptr);

	Matrix3F matrix;
	Matrix3F vp_matrix = Camera2D::GetInstance()->GetMatrix();
	
	const RenderingContext* context = RenderingContext::GetInstance();
	sprite.Bind(m_SpriteLocation, 0);

	for (unsigned int i = 0; i < instances.size(); i++)
	{
		matrix = vp_matrix * instances[i].ToMatrix();
		context->LoadConstantMatrix3F(m_MatrixLocation, 1, GFX_FALSE, &matrix[0][0]);

		context->DrawArray(GFX_TRIANGLES, 0, Quad::GetInstance()->GetVertexCount());
	}
}

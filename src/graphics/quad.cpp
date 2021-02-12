#include <gk/graphics/quad.hpp>

#include <cassert>

/*

V1		 V3
O--------O
|		 |
|		 |
O--------O
V0		V2

*/

#define L -1.0f
#define H  1.0f

#define V0 L, L
#define V1 L, H
#define V2 H, L
#define V3 H, H

const float QUAD_DATA[12] =
{
	V1, V0, V2,
	V3, V1, V2
};

Quad* Quad::Instance = nullptr;

Quad::Quad()
{
	const RenderingContext* context = RenderingContext::GetInstance();

	context->CreateVertexArrays(1, &m_VAO);
	context->BindVertexArray(m_VAO);

	context->CreateAllocations(1, &m_VBO);
	context->BindAllocation(GFX_ARRAY_BUFFER, m_VBO);
	context->CreateBuffer(GFX_ARRAY_BUFFER, sizeof(QUAD_DATA), QUAD_DATA, GFX_STATIC_DRAW);

	context->EnableVertexAttribute(VertexAttributes::VERTEX);
	context->SetVertexAttributeLayoutF(VertexAttributes::VERTEX, 2, GFX_TYPE_FLOAT, false, 0, 0);

	context->BindVertexArray(GFX_NULL);
}

Quad::~Quad()
{
	const RenderingContext* context = RenderingContext::GetInstance();

	if (context->IsVertexArray(m_VAO) == GFX_TRUE)
	{
		context->DeleteVertexArrays(1, &m_VAO);
	}

	if (context->IsAllocation(m_VBO) == GFX_TRUE)
	{
		context->DeleteAllocations(1, &m_VBO);
	}
}

Quad* Quad::CreateInstance()
{
	assert(Instance == nullptr);
	if (Instance == nullptr)
	{
		Instance = new Quad();
	}
    return Instance;
}

void Quad::DeleteInstance()
{
	assert(Instance != nullptr);
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

Quad* Quad::GetInstance()
{
    return Instance;
}

void Quad::Bind()
{
	RenderingContext::GetInstance()->BindVertexArray(m_VAO);
}

unsigned int Quad::GetVertexCount()
{
	return sizeof(QUAD_DATA) * 3 / sizeof(float);
}

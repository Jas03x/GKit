#include <gk/debug_drawer.hpp>

#include <cassert>
#include <cstdio>

#include <gk/camera_3d.hpp>

#include "../shdr/debug_pixel_shader.hpp"
#include "../shdr/debug_vertex_shader.hpp"

DebugDrawer* DebugDrawer::Instance = nullptr;

DebugDrawer::DebugDrawer(unsigned int vertex_limit)
{
    Shader::Load(
        DEBUG_VERTEX_SHADER,
        DEBUG_PIXEL_SHADER,
        [](GFX_HANDLE id)
        {
            const RenderingContext* context = RenderingContext::GetInstance();
			context->SetAttributeLocation(id, VertexAttributes::VERTEX, "vertex");
            context->SetFragmentLocation(id, 0, "pixel");
        }
    );

    m_Enabled = false;
    m_VertexLimit = vertex_limit;
    
    const RenderingContext* context = RenderingContext::GetInstance();

    m_VAO = new VertexArray();
    m_VAO->Bind();

    m_VBO = new VertexBuffer(GFX_ARRAY_BUFFER);
    m_VBO->Bind();
    m_VBO->Allocate(m_VertexLimit * sizeof(Vertex), nullptr, GFX_DYNAMIC_DRAW);

    m_VAO->EnableVertexAttribute(VertexAttributes::VERTEX);
    m_VAO->EnableVertexAttribute(VertexAttributes::COLOUR);
    context->SetVertexAttributeLayoutF(VertexAttributes::VERTEX, 3, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex));
    context->SetVertexAttributeLayoutI(VertexAttributes::COLOUR, 1, GFX_TYPE_UNSIGNED_BYTE,    sizeof(Vertex), (void*)offsetof(Vertex, colour));

    m_Matrix = Shader::GetUniformLocation("matrix");
}

DebugDrawer::~DebugDrawer()
{
}

void DebugDrawer::CreateInstance(unsigned int line_limit)
{
	assert(Instance == nullptr);
	if (Instance == nullptr)
	{
		Instance = new DebugDrawer(line_limit);
	}
}

void DebugDrawer::DeleteInstance()
{
	assert(Instance != nullptr);
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

void DebugDrawer::Enable()
{
    assert(Instance != nullptr);
    Instance->m_Enabled = true;
}

void DebugDrawer::Disable()
{
    assert(Instance != nullptr);
    Instance->m_Enabled = false;
}

bool DebugDrawer::Enabled()
{
    assert(Instance != nullptr);
    return Instance->m_Enabled;
}

void DebugDrawer::DrawLine(const Vertex& v0, const Vertex& v1)
{
    assert(Instance != nullptr);
    if(Instance->m_Enabled)
    {
        Instance->m_LineBuffer.push_back(v0);
        Instance->m_LineBuffer.push_back(v1);
    }
}

void DebugDrawer::Clear()
{
    assert(Instance != nullptr);
    Instance->m_LineBuffer.clear();
}

void DebugDrawer::Render()
{
    assert(Instance != nullptr);
    unsigned int num_vertices = Instance->m_LineBuffer.size();

    const RenderingContext* context = RenderingContext::GetInstance();

    if(Instance->m_Enabled && (num_vertices > 0))
    {
        if(num_vertices < Instance->m_VertexLimit)
        {
            Instance->Shader::Bind();

            Instance->m_VBO->Bind();
            Instance->m_VBO->Update(0, num_vertices * sizeof(Vertex), Instance->m_LineBuffer.data());

            Instance->m_VAO->Bind();

            Matrix4F matrix = Camera3D::GetInstance()->GetMatrix();
            context->LoadConstantMatrix4F(Instance->m_Matrix, 1, GFX_FALSE, &matrix[0][0]);

            context->DrawArray(GFX_LINES, 0, num_vertices);
        }
        else
        {
            printf("error: vertex count %u exceeds limit %u\n", num_vertices, Instance->m_VertexLimit);
        }
        
    }
}

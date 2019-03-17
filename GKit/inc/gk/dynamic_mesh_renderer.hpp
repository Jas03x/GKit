#ifndef GK_DYNAMIC_MESH_RENDERER_H
#define GK_DYNAMIC_MESH_RENDERER_H

#include <vector>

#include <gk/shader.hpp>
#include <gk/dynamic_mesh.hpp>

class DynamicMeshRenderer : public Shader
{
private:
	static DynamicMeshRenderer* Instance;

	uint m_ProjectionMatrix;
	uint m_VertexMatricies;
	uint m_NormalMatricies;
	uint m_DiffuseTexture;
	uint m_SunPosition;
	uint m_SunColor;

private:
	DynamicMeshRenderer();
	~DynamicMeshRenderer();

public:
	static void CreateInstance();
	static void DeleteInstance();

	static void Bind();
	static void Render(const DynamicMesh& mesh);
};

#endif // GK_DYNAMIC_MESH_RENDERER_H

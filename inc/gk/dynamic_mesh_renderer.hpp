#ifndef GK_DYNAMIC_MESH_RENDERER_H
#define GK_DYNAMIC_MESH_RENDERER_H

#include <gk/shader.hpp>
#include <gk/dynamic_mesh.hpp>

class DynamicMeshRenderer : public Shader
{
private:
	static DynamicMeshRenderer* Instance;

	unsigned int m_ProjectionMatrix;
	unsigned int m_VertexMatricies;
	unsigned int m_NormalMatricies;
	unsigned int m_DiffuseTexture;
	unsigned int m_SunPosition;
	unsigned int m_SunColor;
	unsigned int m_CameraPosition;


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

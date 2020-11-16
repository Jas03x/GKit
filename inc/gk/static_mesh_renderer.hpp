#ifndef GK_STATIC_MESH_RENDERER_H
#define GK_STATIC_MESH_RENDERER_H

#include <gk/shader.hpp>
#include <gk/static_mesh.hpp>

class StaticMeshRenderer : public Shader
{
private:
	static StaticMeshRenderer* Instance;

	unsigned int m_ProjectionMatrix;
	unsigned int m_VertexMatricies;
	unsigned int m_NormalMatricies;
	unsigned int m_DiffuseTexture;
	unsigned int m_SunPosition;
	unsigned int m_SunColor;
	unsigned int m_CameraPosition;

private:
	StaticMeshRenderer();
	~StaticMeshRenderer();
    
public:
	static void CreateInstance();
	static void DeleteInstance();

	static void Bind();
	static void Render(const StaticMesh& mesh);
};

#endif // GK_STATIC_MESH_RENDERER_H

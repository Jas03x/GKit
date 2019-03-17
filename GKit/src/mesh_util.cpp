#include <gk/mesh_util.hpp>

#include <math.h>

#define MIN(v0, v1) ((v0) < (v1) ? (v0) : (v1))
#define MAX(v0, v1) ((v0) > (v1) ? (v0) : (v1))

#define NEG_INV -100000.0f
#define POS_INV  100000.0f

void CalculateBounds(const std::vector<MeshData::Vertex>& vertices, Vector3F& min, Vector3F& max)
{
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		const MeshData::Vertex& v = vertices[i];

		min.x = MIN(min.x, v.position.x);
		min.y = MIN(min.y, v.position.y);
		min.z = MIN(min.z, v.position.z);

		max.x = MAX(max.x, v.position.x);
		max.y = MAX(max.y, v.position.y);
		max.z = MAX(max.z, v.position.z);
	}
}

float MeshUtil::GetRadius(const MeshData& data)
{
	Vector3F min = Vector3F(POS_INV, POS_INV, POS_INV);
	Vector3F max = Vector3F(NEG_INV, NEG_INV, NEG_INV);

	for (unsigned int i = 0; i < data.Meshes.size(); i++)
	{
		const MeshData::Mesh& mesh = data.Meshes[i];
		CalculateBounds(mesh.vertices, min, max);
	}

	Vector3F length = Vector3F(abs(max.x - min.x), abs(max.y - min.y), abs(max.z - min.z));
	return sqrt(pow(length.x, 2) + pow(length.y, 2) + pow(length.z, 2)) * 0.5f;
}

float MeshUtil::GetRadius(const MeshData::Mesh& mesh)
{
	Vector3F min = Vector3F(POS_INV, POS_INV, POS_INV);
	Vector3F max = Vector3F(NEG_INV, NEG_INV, NEG_INV);
	
	CalculateBounds(mesh.vertices, min, max);
	
	Vector3F length = Vector3F(abs(max.x - min.x), abs(max.y - min.y), abs(max.z - min.z));
	return sqrt(pow(length.x, 2) + pow(length.y, 2) + pow(length.z, 2)) * 0.5f;
}

Vector3F MeshUtil::GetBoundingBox(const MeshData& data)
{
	Vector3F min = Vector3F(POS_INV, POS_INV, POS_INV);
	Vector3F max = Vector3F(NEG_INV, NEG_INV, NEG_INV);

	for (unsigned int i = 0; i < data.Meshes.size(); i++)
	{
		const MeshData::Mesh& mesh = data.Meshes[i];
		CalculateBounds(mesh.vertices, min, max);
	}

	return Vector3F(abs(max.x - min.x), abs(max.y - min.y), abs(max.z - min.z)) * 0.5f;
}

Vector3F MeshUtil::GetBoundingBox(const MeshData::Mesh& mesh)
{
	Vector3F min = Vector3F(POS_INV, POS_INV, POS_INV);
	Vector3F max = Vector3F(NEG_INV, NEG_INV, NEG_INV);
	
	CalculateBounds(mesh.vertices, min, max);

	return Vector3F(abs(max.x - min.x), abs(max.y - min.y), abs(max.z - min.z)) * 0.5f;
}

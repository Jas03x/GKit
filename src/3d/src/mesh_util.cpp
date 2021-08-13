#include <gk/3d/mesh_util.hpp>

#include <cmath>

#define MIN(v0, v1) ((v0) < (v1) ? (v0) : (v1))
#define MAX(v0, v1) ((v0) > (v1) ? (v0) : (v1))

#define NEG_INV -100000.0f
#define POS_INV  100000.0f

void MeshUtil::CalculateBounds(const std::vector<MeshData::Vertex>& vertices, Vector3F& min, Vector3F& max)
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

void MeshUtil::CalculateBounds(const std::vector<MeshData::Vertex>& vertices, const std::vector<unsigned short>& indices, Vector3F& min, Vector3F& max)
{
	for (unsigned int i = 0; i < indices.size(); i++)
	{
		const MeshData::Vertex& v = vertices[indices[i]];

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
	CalculateBounds(data.vertices, min, max);

	Vector3F length = Vector3F(abs(max.x - min.x), abs(max.y - min.y), abs(max.z - min.z));
	return sqrt(pow(length.x, 2) + pow(length.y, 2) + pow(length.z, 2)) * 0.5f;
}

float MeshUtil::GetRadius(const MeshData& data, unsigned int mesh_index)
{
	const MeshData::Mesh& mesh = data.meshes[mesh_index];

	Vector3F min = Vector3F(POS_INV, POS_INV, POS_INV);
	Vector3F max = Vector3F(NEG_INV, NEG_INV, NEG_INV);
	CalculateBounds(data.vertices, mesh.indices, min, max);
	
	Vector3F length = Vector3F(abs(max.x - min.x), abs(max.y - min.y), abs(max.z - min.z));
	return sqrt(pow(length.x, 2) + pow(length.y, 2) + pow(length.z, 2)) * 0.5f;
}

Vector3F MeshUtil::GetBoundingBox(const MeshData& data)
{
	Vector3F min = Vector3F(POS_INV, POS_INV, POS_INV);
	Vector3F max = Vector3F(NEG_INV, NEG_INV, NEG_INV);
	CalculateBounds(data.vertices, min, max);

	return Vector3F(abs(max.x - min.x), abs(max.y - min.y), abs(max.z - min.z)) * 0.5f;
}

Vector3F MeshUtil::GetBoundingBox(const MeshData& data, unsigned int mesh_index)
{
	const MeshData::Mesh& mesh = data.meshes[mesh_index];

	Vector3F min = Vector3F(POS_INV, POS_INV, POS_INV);
	Vector3F max = Vector3F(NEG_INV, NEG_INV, NEG_INV);
	CalculateBounds(data.vertices, mesh.indices, min, max);

	return Vector3F(abs(max.x - min.x), abs(max.y - min.y), abs(max.z - min.z)) * 0.5f;
}

void MeshUtil::Normalize(MeshData& data)
{
	Vector3F min = Vector3F(POS_INV, POS_INV, POS_INV);
	Vector3F max = Vector3F(NEG_INV, NEG_INV, NEG_INV);
	CalculateBounds(data.vertices, min, max);

	float scale = 1.0f / Vector::Length(max - min);

	for (unsigned int i = 0; i < data.vertices.size(); i++)
	{
		data.vertices[i].position.x *= scale;
		data.vertices[i].position.y *= scale;
		data.vertices[i].position.z *= scale;
	}
}

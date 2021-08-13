#ifndef GK_MESH_UTIL_H
#define GK_MESH_UTIL_H

#include <gk/core/math/linear.hpp>
#include <gk/core/importer/mesh_data.hpp>

namespace MeshUtil
{
	void CalculateBounds(const std::vector<MeshData::Vertex>& vertices, Vector3F& min, Vector3F& max);
	void CalculateBounds(const std::vector<MeshData::Vertex>& vertices, const std::vector<unsigned short>& indices, Vector3F& min, Vector3F& max);

	float GetRadius(const MeshData& data);
	float GetRadius(const MeshData& data, unsigned int mesh_index);

	Vector3F GetBoundingBox(const MeshData& data);
	Vector3F GetBoundingBox(const MeshData& data, unsigned int mesh_index);

	void Normalize(MeshData& data);
}

#endif // GK_MESH_UTIL_H

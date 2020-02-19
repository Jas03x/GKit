#ifndef GK_FBX_IMPORTER_HPP
#define GK_FBX_IMPORTER_HPP

#include <gk/mesh_data.hpp>

namespace FBX
{
	class Importer
	{
	private:
		Importer();
		~Importer();

	public:
		static bool Import(const char* path, MeshData& mesh_data);
	};
}

#endif // GK_FBX_IMPORTER_HPP
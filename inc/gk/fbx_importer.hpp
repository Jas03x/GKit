#ifndef GK_FBX_IMPORTER_HPP
#define GK_FBX_IMPORTER_HPP

#include <vector>

#include <gk/file.hpp>
#include <gk/fbx_format.hpp>
#include <gk/mesh_data.hpp>

namespace FBX
{
	class Importer
	{
	private:
		File* m_file;
		std::vector<Record> m_records;

	private:
		Importer(File* file);
		~Importer();

		bool read_records();

	public:
		static bool Import(const char* path, MeshData& mesh_data);
	};
}

#endif // GK_FBX_IMPORTER_HPP
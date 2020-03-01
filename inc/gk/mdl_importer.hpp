#ifndef GK_MDL_IMPORTER_H
#define GK_MDL_IMPORTER_H

#include <string>
#include <vector>

#include <gk/file.hpp>
#include <gk/data_types.hpp>
#include <gk/matrix.hpp>
#include <gk/mdl_format.hpp>
#include <gk/mesh_data.hpp>

namespace MDL
{
	class Importer
	{
	private:
		File* m_File;
		MeshData* m_Data;

	private:
		Importer();

		bool import(File* file, MeshData* data);
		
		bool read_bone();
		bool read_node();
		bool read_string(std::string& str);
		bool read_array_header(uint16_t& type, uint16_t& length);

		bool read_node_block();
		bool read_material_block();

	public:
		static bool Import(const char* path, MeshData& mesh_data);
	};
};

#endif // GK_MDL_IMPORTER_H

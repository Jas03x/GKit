#ifndef GK_MDL_IMPORTER_H
#define GK_MDL_IMPORTER_H

#include <string>
#include <vector>

#include <gk/io/file.hpp>
#include <gk/context/data_types.hpp>
#include <gk/math/linear.hpp>
#include <gk/importer/mdl_format.hpp>
#include <gk/importer/mesh_data.hpp>

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
		
		bool read_object_header(uint8_t expected_type);
		bool read_object_terminator(uint8_t expected_type);

		bool read_bone();
		bool read_node();
		bool read_mesh();
		bool read_vertex();
		bool read_string(std::string& str);
		bool read_array_header(uint8_t expected_type, uint16_t& length);
		bool read_matrix(Matrix4F& matrix);

		bool read_node_block();
		bool read_mesh_block();
		bool read_material_block();

	public:
		static bool Import(const char* path, MeshData& mesh_data);
	};
};

#endif // GK_MDL_IMPORTER_H

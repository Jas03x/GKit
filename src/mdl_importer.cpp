#include <gk/mdl_importer.hpp>

MDL::Importer::Importer()
{
}

bool MDL::Importer::import(File* file, MeshData* data)
{
	bool status = true;
	m_File = file;
	m_Data = data;

	status = read_node_block();
	if (status) { status = read_material_block(); }
	if (status) { status = read_mesh_block(); }

	return status;
}

bool MDL::Importer::read_object_header(uint8_t expected_type)
{
	bool status = true;

	uint8_t hdr = 0;
	if ((status = m_File->Read(&hdr, 1)))
	{
		if (hdr != expected_type)
		{
			status = false;
			printf("error: expected object of type 0x%hhX\n", expected_type);
		}
	}

	return status;
}

bool MDL::Importer::read_object_terminator(uint8_t expected_type)
{
	bool status = true;
	uint8_t type = MDL::FLAG::TERMINATOR | expected_type;

	uint8_t hdr = 0;
	if ((status = m_File->Read(&hdr, 1)))
	{
		if (hdr != type)
		{
			status = false;
			printf("error: expected end of object type 0x%hhX\n", expected_type);
		}
	}

	return status;
}

bool MDL::Importer::read_array_header(uint8_t expected_type, uint16_t& length)
{
	bool status = read_object_header(expected_type);
	if (status)
	{
		status = m_File->Read(&length, 1);
	}
	return status;
}

bool MDL::Importer::read_mesh_block()
{
	bool status = read_object_header(MDL::MESH_BLOCK);

	if (status)
	{
		uint16_t vertex_count = 0;
		if ((status = read_array_header(MDL::VERTEX_ARRAY, vertex_count)))
		{
			for (unsigned int i = 0; status && (i < vertex_count); i++)
			{
				status = read_vertex();
			}
			
			if (status)
			{
				status = read_object_terminator(MDL::VERTEX_ARRAY);
			}
		}
	}

	if (status)
	{
		uint16_t mesh_count = 0;
		if ((status = read_array_header(MDL::MESH_ARRAY, mesh_count)))
		{
			for (unsigned int i = 0; status && (i < mesh_count); i++)
			{
				status = read_mesh();
			}

			if (status)
			{
				status = read_object_terminator(MDL::MESH_ARRAY);
			}
		}
	}

	if (status)
	{
		status = read_object_terminator(MDL::MESH_BLOCK);
	}

	return status;
}

bool MDL::Importer::read_mesh()
{
	bool status = read_object_header(MDL::MESH);

	if (status)
	{
		m_Data->meshes.push_back(MeshData::Mesh());
		MeshData::Mesh& mesh = m_Data->meshes.back();

		if ((status = read_string(mesh.name)))
		{
			uint16_t index_count = 0;
			if ((status = read_array_header(MDL::INDEX_ARRAY, index_count)) && (index_count > 0))
			{
				m_Data->index_count += index_count;

				mesh.indices.reserve(index_count);
				status = m_File->Read(mesh.indices.data(), sizeof(uint16_t), index_count);
			}

			if (status)
			{
				status = read_object_terminator(MDL::INDEX_ARRAY);
			}
		}
	}

	return status;
}

bool MDL::Importer::read_vertex()
{
	bool status = read_object_header(MDL::VERTEX);

	if (status)
	{
		MDL::Vertex v;
		if ((status = m_File->Read(&v, sizeof(MDL::Vertex), 1)))
		{
			m_Data->vertices.push_back(MeshData::Vertex());
			MeshData::Vertex& vertex = m_Data->vertices.back();

			vertex.position = Vector3F(v.position[0], v.position[1], v.position[2]);
			vertex.normal = Vector3F(v.normal[0], v.normal[1], v.normal[2]);
			vertex.uv = Vector2F(v.uv[0], v.uv[1]);
			vertex.node_index = v.node_index;
			for (unsigned int i = 0; i < 4; i++) {
				vertex.bone_indices[i] = v.bone_indices[i];
				vertex.bone_weights[i] = v.bone_weights[i];
			}
			vertex.bone_count = v.bone_count;
		}
	}

	return status;
}

bool MDL::Importer::read_node_block()
{
	bool status = read_object_header(MDL::NODE_BLOCK);

	if (status)
	{
		uint16_t node_count = 0;
		if ((status = read_array_header(MDL::NODE_ARRAY, node_count)))
		{
			if (node_count == 0)
			{
				status = false;
				printf("error: node array empty\n");
			}
		}

		for (unsigned int i = 0; status && (i < node_count); i++)
		{
			status = read_node();
		}

		if (status)
		{
			status = read_object_terminator(MDL::NODE_ARRAY);
		}
	}

	if (status)
	{
		uint16_t bone_count = 0;
		if ((status = read_array_header(MDL::BONE_ARRAY, bone_count)))
		{
			for (unsigned int i = 0; status && (i < bone_count); i++)
			{
				status = read_bone();
			}
		}

		if (status)
		{
			status = read_object_terminator(MDL::BONE_ARRAY);
		}
	}

	if (status)
	{
		status = read_object_terminator(MDL::NODE_BLOCK);
	}

	return status;
}

bool MDL::Importer::read_material_block()
{
	bool status = read_object_header(MDL::MATERIAL_BLOCK);
	
	if (status) {
		status = read_string(m_Data->diffuse_texture);
	}

	if (status) {
		status = read_object_terminator(MDL::MATERIAL_BLOCK);
	}

	return status;
}

bool MDL::Importer::read_bone()
{
	bool status = read_object_header(MDL::BONE);

	m_Data->bones.push_back(MeshData::Bone());
	MeshData::Bone& bone = m_Data->bones.back();

	if ((status = read_string(bone.name)))
	{
		status = read_matrix(bone.bind_pose_matrix);
	}

	return status;
}

bool MDL::Importer::read_matrix(Matrix4F& matrix)
{
	bool status = read_object_header(MDL::MATRIX);
	if (status)
	{
		status = m_File->Read(&matrix[0][0], 16);
	}
	return status;
}

bool MDL::Importer::read_node()
{
	bool status = read_object_header(MDL::NODE);

	m_Data->nodes.push_back(MeshData::Node());
	MeshData::Node& node = m_Data->nodes.back();

	if ((status = read_string(node.name)))
	{
		status = read_string(node.parent);
	}

	if (status)
	{
		status = read_matrix(node.offset_matrix);
	}

	return status;
}

bool MDL::Importer::read_string(std::string& str)
{
	bool status = read_object_header(MDL::STRING);

	uint8_t length = 0;
	m_File->Read(&length, 1);
	if (length == 0)
	{
		str.clear();
		m_File->Seek(FILE_CUR, 1);
	}
	else
	{
		if (status)
		{
			str.resize(length, 0);
			status = m_File->Read(&str[0], length);
		}
	}

	return status;
}

bool MDL::Importer::Import(const char* path, MeshData& mesh_data)
{
	bool status = true;
	
	File file(path, File::READ_BINARY);
	if (!file.IsOpen()) {
		status = false;
	}

	if (status)
	{
		uint32_t signature = 0;
		if ((status = file.Read(&signature, 1)))
		{
			if (signature != MDL::SIGNATURE)
			{
				status = false;
				printf("error: file is not a valid mdl file\n");
			}
		}
	}

	if (status)
	{
		MDL::Importer importer;
		status = importer.import(&file, &mesh_data);
	}

	if (status)
	{
		uint32_t eof_signature = 0;
		if ((status = file.Read(&eof_signature, 1)))
		{
			if (eof_signature != MDL::END_OF_FILE)
			{
				status = false;
				printf("error: expected end of file\n");
			}
		}
	}

	return status;
}

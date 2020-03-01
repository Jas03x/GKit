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

	return status;
}

bool MDL::Importer::read_array_header(uint16_t& type, uint16_t& length)
{
	bool status = true;

	if ((status = m_File->Read(&type, 1)))
	{
		status = m_File->Read(&length, 1);
	}

	return status;
}

bool MDL::Importer::read_node_block()
{
	bool status = true;

	uint16_t type = 0, length = 0;
	if ((status = read_array_header(type, length)))
	{
		if (type != MDL::NODE_ARRAY)
		{
			status = false;
			printf("error: expected node array\n");
		}
		else if (length == 0)
		{
			status = false;
			printf("error: node array empty\n");
		}
	}

	for (unsigned int i = 0; status && (i < length); i++)
	{
		status = read_node();
	}

	return status;
}

bool MDL::Importer::read_material_data()
{
	bool status = true;

	return status;
}

bool MDL::Importer::read_bone()
{
	bool status = true;

	MeshData::Bone bone;

	if ((status = read_string(bone.name)))
	{
		status = m_File->Read(&bone.bind_pose_matrix[0][0], 16);
	}

	return status;
}

bool MDL::Importer::read_node()
{
	bool status = true;

	MeshData::Node node;

	if ((status = read_string(node.name)))
	{
		status = read_string(node.parent);
	}

	if (status)
	{
		status = m_File->Read(&node.offset_matrix[0][0], 16);
	}

	return status;
}

bool MDL::Importer::read_string(std::string& str)
{
	bool status = true;

	uint16_t length = 0;
	m_File->Read(&length, 1);
	if (length == 0)
	{
		status = false;
		printf("error: invalid length\n");
	}

	if (status)
	{
		str.resize(length, 0);
		status = m_File->Read(&str[0], length);
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
		uint64_t signature = 0;
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
		MDL::Importer importer();
		status = importer.import(&file, &mesh_data);
	}

	return status;
}

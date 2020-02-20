#include <gk/fbx_importer.hpp>

#include <gk/file.hpp>

// see: https://code.blender.org/2013/08/fbx-binary-file-format-specification/

const uint8_t FBX_HDR[] = { 0x4B, 0x61, 0x79, 0x64, 0x61, 0x72, 0x61, 0x20, 0x46, 0x42, 0x58, 0x20, 0x42, 0x69, 0x6E, 0x61, 0x72, 0x79, 0x20, 0x20, 0x00, 0x1A, 0x00 };

FBX::Importer::Importer(File* file)
{
	m_file = file;
}

FBX::Importer::~Importer()
{

}

bool FBX::Importer::read_records()
{
	bool status = true;

	m_records.push_back(Record());
	Record& r = m_records.back();

	m_file->Read(&r.end_offset, sizeof(uint32_t), 1);
	m_file->Read(&r.num_properties, sizeof(uint32_t), 1);
	m_file->Read(&r.property_list_len, sizeof(uint32_t), 1);
	m_file->Read(&r.name_len, sizeof(uint8_t), 1);

	r.name = std::string(r.name_len + 1, 0);
	m_file->Read(&r.name[0], sizeof(uint8_t), r.name_len);

	printf("Record: %s\n", r.name.c_str());

	for (unsigned int i = 0; i < r.num_properties; i++)
	{
		r.properties.push_back(Property());
		Property& p = r.properties.back();

		char type = 0;
		m_file->Read(&type, sizeof(uint8_t), 1);

		switch (type)
		{
			case 'Y': { p.type = FBX::Property::INT16;  break; }
			case 'C': { p.type = FBX::Property::BOOL;   break; }
			case 'I': { p.type = FBX::Property::INT32;  break; }
			case 'F': { p.type = FBX::Property::FLOAT;  break; }
			case 'D': { p.type = FBX::Property::DOUBLE; break; }
			case 'L': { p.type = FBX::Property::INT64;  break; }
			case 'f': { p.type = FBX::Property::FLOAT_ARRAY;  break; }
			case 'd': { p.type = FBX::Property::DOUBLE_ARRAY; break; }
			case 'l': { p.type = FBX::Property::INT64_ARRAY;  break; }
			case 'i': { p.type = FBX::Property::INT32_ARRAY;  break; }
			case 'b': { p.type = FBX::Property::BOOL_ARRAY;   break; }
			case 'S': { p.type = FBX::Property::STRING;     break; }
			case 'R': { p.type = FBX::Property::RAW_BINARY; break; }
			default:
			{
				status = false;
				printf("error: unknown data type '%c'\n", type);
				break;
			}
		}

		if (status)
		{
			if ((p.type & FBX::Property::PRIMITIVE) != 0x0)
			{
				switch (p.type)
				{
					case FBX::Property::INT16:  { m_file->Seek(FILE_CUR, sizeof(uint16_t)); break; }
					case FBX::Property::BOOL:   { m_file->Seek(FILE_CUR, sizeof(uint8_t));  break; }
					case FBX::Property::INT32:  { m_file->Seek(FILE_CUR, sizeof(uint32_t)); break; }
					case FBX::Property::FLOAT:  { m_file->Seek(FILE_CUR, sizeof(float));    break; }
					case FBX::Property::DOUBLE: { m_file->Seek(FILE_CUR, sizeof(double));   break; }
					case FBX::Property::INT64:  { m_file->Seek(FILE_CUR, sizeof(uint64_t)); break; }
				}
			}
			else if ((p.type & FBX::Property::ARRAY))
			{
				uint32_t array_length = 0;
				uint32_t array_encoding = 0;
				uint32_t compressed_length = 0;
				m_file->Read(&array_length, sizeof(uint32_t), 1);
				m_file->Read(&array_encoding, sizeof(uint32_t), 1);
				m_file->Read(&compressed_length, sizeof(uint32_t), 1);

				switch (p.type)
				{
					case FBX::Property::FLOAT_ARRAY:  { m_file->Seek(FILE_CUR, array_encoding == 1 ? compressed_length : array_length * sizeof(float));    break; }
					case FBX::Property::DOUBLE_ARRAY: { m_file->Seek(FILE_CUR, array_encoding == 1 ? compressed_length : array_length * sizeof(double));   break; }
					case FBX::Property::INT64_ARRAY:  { m_file->Seek(FILE_CUR, array_encoding == 1 ? compressed_length : array_length * sizeof(uint64_t)); break; }
					case FBX::Property::INT32_ARRAY:  { m_file->Seek(FILE_CUR, array_encoding == 1 ? compressed_length : array_length * sizeof(uint32_t)); break; }
					case FBX::Property::BOOL_ARRAY:   { m_file->Seek(FILE_CUR, array_encoding == 1 ? compressed_length : array_length * sizeof(uint8_t));  break; }
				}
			}
			else
			{
				uint32_t length = 0;
				m_file->Read(&length, sizeof(uint32_t), 1);
				m_file->Seek(FILE_CUR, length);
			}
		}
	}

	if (m_file->Tell() < r.end_offset) // there is a nested list
	{
		status = read_records();
	}

	return true;
}

bool FBX::Importer::Import(const char* path, MeshData& mesh_data)
{
	File file(path, "rb");
	if (!file.IsOpen())
	{
		return false;
	}
	
	bool status = true;

	uint8_t header[23];
	if((status = file.Read(header, sizeof(uint8_t), 23)))
	{
		status = (memcmp(header, FBX_HDR, sizeof(FBX_HDR)) == 0);
	}

	uint32_t version = 0;
	if ((status = file.Read(&version, sizeof(uint32_t), 1)))
	{
		printf("FBX Version %u\n", version);
	}

	if (status)
	{
		FBX::Importer importer(&file);

		long int file_size = file.Size();
		while (status && (file.Tell() < file_size))
		{
			importer.read_records();
		}
	}

	return status;
}

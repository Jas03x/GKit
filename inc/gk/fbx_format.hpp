#ifndef GK_FBX_FORMAT_HPP
#define GK_FBX_FORMAT_HPP

#include <string>
#include <vector>

#include <gk/data_types.hpp>

namespace FBX
{
	struct Property
	{
		enum TYPE
		{
			PRIMITIVE = 0x1,
			ARRAY     = 0x2,
			OTHER     = 0x4
		};

		enum SUBTYPE
		{
			// PRIMITIVES
			INT16  = (PRIMITIVE << 8) | 0x1,
			BOOL   = (PRIMITIVE << 8) | 0x2,
			INT32  = (PRIMITIVE << 8) | 0x3,
			FLOAT  = (PRIMITIVE << 8) | 0x4,
			DOUBLE = (PRIMITIVE << 8) | 0x5,
			INT64  = (PRIMITIVE << 8) | 0x6,
			// ARRAYS
			FLOAT_ARRAY  = (ARRAY << 8) | 0x7,
			DOUBLE_ARRAY = (ARRAY << 8) | 0x8,
			INT64_ARRAY  = (ARRAY << 8) | 0x9,
			INT32_ARRAY  = (ARRAY << 8) | 0xA,
			BOOL_ARRAY   = (ARRAY << 8) | 0xB,
			// OTHER:
			STRING     = (OTHER << 8) | 0xC,
			RAW_BINARY = (OTHER << 8) | 0xD
		};

		struct ArrayData
		{
			uint32_t array_length;
			uint32_t encoding;
			uint32_t compressed_length;
			// data
		};

		struct SpecialData
		{
			uint32_t length;
			// data
		};

		uint16_t type;
	};

	struct Record
	{
		uint32_t end_offset;
		uint32_t num_properties;
		uint32_t property_list_len;
		uint8_t  name_len;

		std::string name;
		std::vector<Property> properties;
		std::vector<Record*> child_records;
	};
}

#endif // GK_FBX_FORMAT_HPP
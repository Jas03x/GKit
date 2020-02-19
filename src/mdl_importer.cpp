#include <gk/mdl_importer.hpp>

#include <assert.h>

#include <map>

#include <gk/file.hpp>
#include <gk/mdl_format.hpp>

#if 0
	#define DEBUG_PRINT(frmt, ...) printf(frmt, __VA_ARGS__);
#else
	#define DEBUG_PRINT(frmt, ...)
#endif

unsigned int MDL_Importer::ReadTextures(const byte* data)
{
	unsigned int offset = 0;
	
	const MDL_Array* array = reinterpret_cast<const MDL_Array*>(data);
	assert(array->type == TEXTURE_ARRAY);
	offset += sizeof(MDL_Array);

	for(unsigned int i = 0; i < array->length; i++)
	{
		const MDL_Texture* texture = reinterpret_cast<const MDL_Texture*>(data + offset);
		offset += sizeof(MDL_Texture);

		this->Textures.push_back(std::string(texture->path.string));
		DEBUG_PRINT("Texture: %s\n", this->Textures.back().c_str());
	}

	return offset;
}

unsigned int MDL_Importer::ReadMaterials(const byte* data)
{
	unsigned int offset = 0;

	const MDL_Array* array = reinterpret_cast<const MDL_Array*>(data);
	assert(array->type == MATERIAL_ARRAY);
	offset += sizeof(MDL_Array);

	for(unsigned int i = 0; i < array->length; i++)
	{
		const MDL_Material* material = reinterpret_cast<const MDL_Material*>(data + offset);
		offset += sizeof(MDL_Material);

		Materials.push_back(MDL_Importer::Material());

		MDL_Importer::Material& mat = Materials.back();
		memcpy(mat.diffuse.values,  material->color.values,    sizeof(Vector3F));
		memcpy(mat.specular.values, material->specular.values, sizeof(Vector3F));
		mat.texture_index = material->texture_index;

		DEBUG_PRINT("Material: ((%f, %f, %f), (%f, %f, %f), %hhu)\n",
			mat.diffuse.x,  mat.diffuse.y,  mat.diffuse.z,
			mat.specular.x, mat.specular.y, mat.specular.z,
			mat.texture_index
		);
	}

	return offset;
}

unsigned int MDL_Importer::ReadNodes(const byte* data)
{
	unsigned int offset = 0;

	const MDL_Array* array = reinterpret_cast<const MDL_Array*>(data);
	assert(array->type == NODE_ARRAY);
	offset += sizeof(MDL_Array);

	for(unsigned int i = 0; i < array->length; i++)
	{
		const MDL_Node* node = reinterpret_cast<const MDL_Node*>(data + offset);
		offset += sizeof(MDL_Node);

		Nodes.push_back(MDL_Importer::Node());

		MDL_Importer::Node& n = Nodes.back();
		n.name   = std::string(node->name.string);
		n.parent = std::string(node->parent.string);
		memcpy(n.offset_matrix.values, node->offset_matrix.values, sizeof(float) * 16);

		DEBUG_PRINT("Node %s => %s\n[%f, %f, %f, %f]\n\t[%f, %f, %f, %f]\n\t[%f, %f, %f, %f]\n\t[%f, %f, %f, %f]\n",
			n.name.c_str(), n.parent.c_str(),
			n.offset_matrix[0][0], n.offset_matrix[0][1], n.offset_matrix[0][2], n.offset_matrix[0][3],
			n.offset_matrix[1][0], n.offset_matrix[1][1], n.offset_matrix[1][2], n.offset_matrix[1][3],
			n.offset_matrix[2][0], n.offset_matrix[2][1], n.offset_matrix[2][2], n.offset_matrix[2][3],
			n.offset_matrix[3][0], n.offset_matrix[3][1], n.offset_matrix[3][2], n.offset_matrix[3][3]
		);
	}

	return offset;
}

unsigned int MDL_Importer::ReadBones(const byte* data)
{
	unsigned int offset = 0;

	const MDL_Array* array = reinterpret_cast<const MDL_Array*>(data);
	assert(array->type == BONE_ARRAY);
	offset += sizeof(MDL_Array);

	for(unsigned int i = 0; i < array->length; i++)
	{
		const MDL_Bone* bone = reinterpret_cast<const MDL_Bone*>(data + offset);
		offset += sizeof(MDL_Bone);

		Bones.push_back(MDL_Importer::Bone());

		MDL_Importer::Bone& b = Bones.back();
		b.name = std::string(bone->name.string);
		memcpy(b.offset_matrix.values, bone->offset_matrix.values, sizeof(Matrix4F));

		DEBUG_PRINT("Bone %s:\n", b.name.c_str());
		DEBUG_PRINT("Matrix:\n{ %f, %f, %f, %f }\n{ %f, %f, %f, %f }\n{ %f, %f, %f, %f }\n{ %f, %f, %f, %f }\n",
			b.offset_matrix[0][0], b.offset_matrix[0][1], b.offset_matrix[0][2], b.offset_matrix[0][3],
			b.offset_matrix[1][0], b.offset_matrix[1][1], b.offset_matrix[1][2], b.offset_matrix[1][3],
			b.offset_matrix[2][0], b.offset_matrix[2][1], b.offset_matrix[2][2], b.offset_matrix[2][3],
			b.offset_matrix[3][0], b.offset_matrix[3][1], b.offset_matrix[3][2], b.offset_matrix[3][3]
		);
		
		if(bone->animation.num_frames > 0)
		{
			b.animation.frames.reserve(bone->animation.num_frames);

			for(unsigned int j = 0; j < bone->animation.num_frames; j++)
			{
				const MDL_Frame* frame = reinterpret_cast<const MDL_Frame*>(data + offset);
				offset += sizeof(MDL_Frame);

				b.animation.frames.push_back(MDL_Importer::Frame());

				MDL_Importer::Frame& f = b.animation.frames.back();
				f.time = frame->time;
				memcpy(f.position.values, frame->position.values, sizeof(Vector3F));
				memcpy(f.rotation.values, frame->rotation.values, sizeof(Vector4F));
				memcpy(f.scale.values,    frame->scale.values,    sizeof(Vector3F));

				DEBUG_PRINT("Animation frame: Time = %f, Transformation = ((%f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f))\n",
					f.time,
					f.position.x, f.position.y, f.position.z,
					f.rotation.x, f.rotation.y, f.rotation.z, f.rotation.w,
					f.scale.x,    f.scale.y,    f.scale.z
				);
			}
		}
	}

	return offset;
}

unsigned int MDL_Importer::ReadMeshes(const byte* data)
{
	unsigned int offset = 0;
	unsigned int vertex_offset = 0; // each mesh has its own set of vertices, but the vertices are all stored in one big array. So we need to store an offset into the combined array

	const MDL_Array* array = reinterpret_cast<const MDL_Array*>(data);
	assert(array->type == MESH_ARRAY);
	offset += sizeof(MDL_Array);

	for(unsigned int i = 0; i < array->length; i++)
	{
		const MDL_Mesh* mesh = reinterpret_cast<const MDL_Mesh*>(data + offset);
		offset += sizeof(MDL_Mesh);

		Meshes.push_back(MDL_Importer::Mesh());

		MDL_Importer::Mesh& m = Meshes.back();
		m.name = std::string(mesh->name.string);
		m.vertices.reserve(mesh->vertex_array_length);
		m.indices.reserve(mesh->triangle_array_length * 3);

		IndexCount  += mesh->triangle_array_length * 3;
		VertexCount += mesh->vertex_array_length;

		DEBUG_PRINT("Mesh %s:\n", m.name.c_str());

		for(unsigned int j = 0; j < mesh->vertex_array_length; j++)
		{
			const MDL_Vertex* vertex = reinterpret_cast<const MDL_Vertex*>(data + offset);
			offset += sizeof(MDL_Vertex);

			m.vertices.push_back(MDL_Importer::Vertex());
			
			MDL_Importer::Vertex& v = m.vertices.back();
			memcpy(v.position.values, vertex->position.values, sizeof(Vector3F));
			memcpy(v.normal.values,   vertex->normal.values,   sizeof(Vector3F));
			memcpy(v.uv.values,       vertex->uv.values,       sizeof(Vector2F));
			v.node     = vertex->node_index;
			v.material = vertex->material;
			for(unsigned int k = 0; k < 4; k++)
			{
				v.bones[k].index  = vertex->bone_indices.values[k];
				v.bones[k].weight = vertex->bone_weights.values[k];
			}

			DEBUG_PRINT("Vertex: ((%f, %f, %f), (%f, %f, %f), (%f, %f), %hhu, %hhu, ((%hhu, %f), (%hhu, %f), (%hhu, %f), (%hhu, %f)))\n",
				v.position.x, 	  v.position.y,		v.position.z,
				v.normal.x,   	  v.normal.y,   	v.normal.z,
				v.uv.x,       	  v.uv.y,
				v.node,       	  v.material,
				v.bones[0].index, v.bones[0].weight,
				v.bones[1].index, v.bones[1].weight,
				v.bones[2].index, v.bones[2].weight,
				v.bones[3].index, v.bones[3].weight
			);
		}

		for(unsigned int j = 0; j < mesh->triangle_array_length; j++)
		{
			const MDL_Triangle* triangle = reinterpret_cast<const MDL_Triangle*>(data + offset);
			offset += sizeof(MDL_Triangle);

			m.indices.push_back(vertex_offset + triangle->indices[0]);
			m.indices.push_back(vertex_offset + triangle->indices[1]);
			m.indices.push_back(vertex_offset + triangle->indices[2]);

			DEBUG_PRINT("Triangle: (%hu, %hu, %hu)\n",
				triangle->indices[0],
				triangle->indices[1],
				triangle->indices[2]
			);
		}

		vertex_offset += mesh->vertex_array_length;
	}

	return offset;
}

MDL_Importer::MDL_Importer(const char* path)
{
	File file(path, "rb");

	MDL_Header header;
	
	file.Read(&header, sizeof(MDL_Header), 1);
	assert(header.signature == MDL_SIGNATURE);

	file.Seek(FILE_END);
	unsigned int size = file.Tell();
	file.Seek(FILE_SET);

	std::vector<byte> buffer;
	buffer.reserve(size);

	file.Read(buffer.data(), 1, size);
	file.Close();

	IndexCount  = 0;
	VertexCount = 0;

	unsigned int offset = sizeof(MDL_Header); // since we already read the header
	offset += ReadTextures(buffer.data() + offset);
	offset += ReadMaterials(buffer.data() + offset);
	offset += ReadNodes(buffer.data() + offset);
	offset += ReadBones(buffer.data() + offset);
	offset += ReadMeshes(buffer.data() + offset);

	assert(offset == size);
}

MDL_Importer::~MDL_Importer()
{

}

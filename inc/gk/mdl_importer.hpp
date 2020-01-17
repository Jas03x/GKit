#ifndef GK_MDL_IMPORTER_H
#define GK_MDL_IMPORTER_H

#include <string>
#include <vector>

#include <gk/data_types.hpp>
#include <gk/matrix.hpp>

class MDL_Importer
{
private:
	unsigned int ReadTextures(const byte* data);
	unsigned int ReadMaterials(const byte* data);
	unsigned int ReadNodes(const byte* data);
	unsigned int ReadBones(const byte* data);
	unsigned int ReadMeshes(const byte* data);

public:
	struct Material
	{
		Vector3F diffuse;
		Vector3F specular;
		unsigned char texture_index;
	};

	struct Node
	{
		std::string name;
		std::string parent;
	
		Matrix4F    offset_matrix;
	};

	struct Frame
	{
		float    time;

		Vector3F position;
		Vector4F rotation;
		Vector3F scale;
	};

	struct Animation
	{
		std::vector<Frame> frames;
	};

	struct Bone
	{
		std::string name;
		Matrix4F    offset_matrix;
		Animation   animation;
	};

	struct BoneWeight
	{
		unsigned char index;
		float         weight;
	};

	struct Vertex
	{
		Vector3F      position;
		Vector3F      normal;
		Vector2F      uv;
		unsigned char node;
		unsigned char material;
		BoneWeight 	  bones[4];
	};

	struct Mesh
	{
		std::string 				name;
		std::vector<Vertex> 		vertices;
		std::vector<unsigned short> indices;
	};

	std::vector<Node> Nodes;
	std::vector<Mesh> Meshes;
	std::vector<Bone> Bones;

	std::vector<Material> Materials;
	std::vector<std::string> Textures;

	unsigned int VertexCount;
	unsigned int IndexCount;

	MDL_Importer(const char* path);
	~MDL_Importer();
};

#endif // GK_MDL_IMPORTER_H

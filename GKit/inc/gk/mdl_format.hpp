#ifndef GK_MDL_FORMAT_HPP
#define GK_MDL_FORMAT_HPP

enum { MDL_SIGNATURE = 0x4D444C00 }; // 'MDL'
enum { MDL_MAX_STR_LEN = 32 };

enum
{
    UNKNOWN_ARRAY  = 0,
    TEXTURE_ARRAY  = 1,
    MATERIAL_ARRAY = 2,
    NODE_ARRAY     = 3,
    BONE_ARRAY     = 4,
    MESH_ARRAY     = 5
};

struct MDL_Header
{
    unsigned int signature;
};

struct MDL_Vector2F { float          values[2];    };
struct MDL_Vector3F { float          values[3];    };
struct MDL_Vector4U { unsigned short values[4];    };
struct MDL_Vector4F { float          values[4];    };
struct MDL_Matrix4F { float          values[4][4]; };

struct MDL_String
{
    char string[MDL_MAX_STR_LEN];
};

struct MDL_Texture
{
    MDL_String path;
};

struct MDL_Array
{
    unsigned char type;
    unsigned int  length;
};

struct MDL_Material
{
    MDL_Vector3F color;
    MDL_Vector3F specular;
    unsigned char texture_index;
};

struct MDL_Node
{
    MDL_String   name;
    MDL_String   parent;

	MDL_Matrix4F offset_matrix;
};

struct MDL_Frame
{
    float time;

    MDL_Vector3F position;
    MDL_Vector4F rotation;
    MDL_Vector3F scale;
};

struct MDL_Animation
{
    unsigned int num_frames;
    // frames ...
};

struct MDL_Bone
{
    MDL_String    name;
    MDL_Matrix4F  offset_matrix;
    MDL_Animation animation;
};

struct MDL_Vertex
{
    MDL_Vector3F position;
    MDL_Vector3F normal;
    MDL_Vector2F uv;
    unsigned char node_index;
    MDL_Vector4U bone_indices;
    MDL_Vector4F bone_weights;
    unsigned char material;
    unsigned char bone_count;
};

struct MDL_Triangle { unsigned short indices[3]; };

struct MDL_Mesh
{
    MDL_String   name;
    unsigned int vertex_array_length;
    unsigned int triangle_array_length;
    
    // vertices  ...
    // triangles ...
};

#endif // GK_MDL_FORMAT_HPP

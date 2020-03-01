#ifndef GK_MDL_FORMAT_HPP
#define GK_MDL_FORMAT_HPP

/*
* _______________________________________
* |           MDL File Format           |
* |-------------------------------------|
* |    Type         |   Description     |
* |-----------------|-------------------|
* |    UInt32       |   Signature       |
* |-----------------|-------------------|
* |    MDL Block    |   Node data       |
* |-----------------|-------------------|
* |    MDL Block    |   Material data   |
* |-----------------|-------------------|
* |    MDL Block    |   Mesh data       |
* |_________________|___________________|
*
* ______________________________________
* |         MDL String Format           |
* |-------------------------------------|
* |    Type         |  Description      |
* |-----------------|-------------------|
* |    UInt8        |  String Signature |
* |-----------------|-------------------|
* |    UInt8        |  Length           |
* |-----------------|-------------------|
* |    char[]       |  Data             |
* |_________________|___________________|
* * Note: The string is null terminated, and the size includes the terminator.
* 
* _______________________________________
* |           MDL Array Format          |
* |-------------------------------------|
* |    Type         |   Description     |
* |-----------------|-------------------|
* |    UInt8        |   Array Signature |
* |-----------------|-------------------|
* |    UInt8        |   Type            |
* |-----------------|-------------------|
* |    UInt16       |   Length          |
* |-----------------|-------------------|
* |               Data                  |
* |-----------------|-------------------|
* |    UInt8        |   End Of Array    |
* |_________________|___________________|
*
* _______________________________________
* |     MDL Node Data Block Format      |
* |-------------------------------------|
* |    Type         |   Description     |
* |-----------------|-------------------|
* |    UInt8        |   Block Signature |
* |-----------------|-------------------|
* |    MDL Array    |   Nodes           |
* |-----------------|-------------------|
* |    MDL Array    |   Bones           |
* |-----------------|-------------------|
* |    UInt8        |   End Of Block    |
* |_________________|___________________|
*
* _______________________________________
* |   MDL Material Data Block Format    |
* |-------------------------------------|
* |    Type         |   Description     |
* |-----------------|-------------------|
* |    UInt8        |   Block Signature |
* |-----------------|-------------------|
* |    MDL String   |   Texture         |
* |-----------------|-------------------|
* |    UInt8        |   End Of Block    |
* |_________________|___________________|
*
* _______________________________________
* |     MDL Mesh Data Block Format      |
* |-------------------------------------|
* |    Type         |   Description     |
* |-----------------|-------------------|
* |    UInt8        |   Block Signature |
* |-----------------|-------------------|
* |    MDL Array    |   Vertices        |
* |-----------------|-------------------|
* |    MDL Array    |   Mesh Data       |
* |-----------------|-------------------|
* |    UInt8        |   End Of Block    |
* |_________________|___________________|
*
* _______________________________________
* |           MDL Node Format           |
* |-------------------------------------|
* |    Type         |   Description     |
* |-----------------|-------------------|
* |    UInt8        |   Node Signature  |
* |-----------------|-------------------|
* |    MDL String   |   Name            |
* |-----------------|-------------------|
* |    MDL String   |   Parent          |
* |-----------------|-------------------|
* |    MDL Matrix4F |   Transform       |
* |_________________|___________________|
*
* _______________________________________
* |           MDL Bone Format           |
* |-------------------------------------|
* |    Type         |   Description     |
* |-----------------|-------------------|
* |    UInt8        |   Bone Signature  |
* |-----------------|-------------------|
* |    MDL String   |   Name            |
* |-----------------|-------------------|
* |    MDL Matrix4F |   Offset matrix   |
* |_________________|___________________|
*
* _______________________________________
* |           MDL Vertex Format         |
* |-------------------------------------|
* |    Type         |  Description      |
* |-----------------|-------------------|
* |    UInt8        |  Vertex Signature |
* |-----------------|-------------------|
* |    MDL Vector3F |  Position         |
* |-----------------|-------------------|
* |    MDL Vector3F |  Normal           |
* |-----------------|-------------------|
* |    MDL Vector2F |  UV               |
* |-----------------|-------------------|
* |    MDL Vector4U |  Bone indices     |
* |-----------------|-------------------|
* |    MDL Vector4F |  Bone weights     |
* |-----------------|-------------------|
* |    UInt8        |  Bone count       |
* |_________________|___________________|
*
* _______________________________________
* |           MDL Mesh Format           |
* |-------------------------------------|
* |    Type         |   Description     |
* |-----------------|-------------------|
* |    UInt8        |   Mesh Signature  |
* |-----------------|-------------------|
* |    MDL String   |   Name            |
* |-----------------|-------------------|
* |    MDL Array    |   Indices         |
* |_________________|___________________|
*
*/

namespace MDL
{
    enum
    {
        SIGNATURE   = 0x4D444C00, // 'MDL'
        END_OF_FILE = 0x464F4500  // 'EOF'
    };

    enum FLAG
    {
        CLASS = 0x20,
        ARRAY = 0x40,
        BLOCK = 0x60,
        TERMINATOR = 0x80
    };

    enum ID
    {
        STRING   = 0x1,
        NODE     = 0x2,
        BONE     = 0x3,
        VERTEX   = 0x4,
        MATERIAL = 0x5,
        MESH     = 0x6,
        INDEX    = 0x7
    };

    enum
    {
        NODE_BLOCK     = FLAG::BLOCK | ID::NODE,
        MATERIAL_BLOCK = FLAG::BLOCK | ID::MATERIAL,
        MESH_BLOCK     = FLAG::BLOCK | ID::MESH,
        NODE_ARRAY     = FLAG::ARRAY | ID::NODE,
        BONE_ARRAY     = FLAG::ARRAY | ID::BONE,
        VERTEX_ARRAY   = FLAG::ARRAY | ID::VERTEX,
        INDEX_ARRAY    = FLAG::ARRAY | ID::INDEX,
        STRING         = FLAG::CLASS | ID::STRING,
        NODE           = FLAG::CLASS | ID::NODE,
        BONE           = FLAG::CLASS | ID::BONE,
        VERTEX         = FLAG::CLASS | ID::VERTEX,
        MESH           = FLAG::CLASS | ID::MESH
    };

    struct Vector2F { float          values[2]; };
    struct Vector3F { float          values[3]; };
    struct Vector4U { unsigned short values[4]; };
    struct Vector4F { float          values[4]; };
    struct Matrix4F { float          values[4][4]; };

    struct Vertex
    {
        Vector3F position;
        Vector3F normal;
        Vector2F uv;
        Vector4U bone_indices;
        Vector4U bone_weights;
        uint8_t  bone_count;
    };
}

#endif // GK_MDL_FORMAT_HPP

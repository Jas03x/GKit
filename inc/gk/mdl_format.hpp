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
* |-----------------|-------------------|
* |    UInt32       |   End of File     |
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
* |    float[16]    |   Offset matrix   |
* |_________________|___________________|
*
* _______________________________________
* |           MDL Vertex Format         |
* |-------------------------------------|
* |    Type         |  Description      |
* |-----------------|-------------------|
* |    UInt8        |  Vertex Signature |
* |-----------------|-------------------|
* |    float[3]     |  Position         |
* |-----------------|-------------------|
* |    float[3]     |  Normal           |
* |-----------------|-------------------|
* |    float[2]     |  UV               |
* |-----------------|-------------------|
* |    UInt8        |  Node index       |
* |-----------------|-------------------|
* |    Uint8[4]     |  Bone indices     |
* |-----------------|-------------------|
* |    float[8]     |  Bone weights     |
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
        END_OF_FILE = 0x454F4600  // 'EOF'
    };

    enum FLAG
    {
        CLASS = 0x20,
        ARRAY = 0x40,
        BLOCK = 0x60,
        TERMINATOR = 0x80
    };

    enum
    {
        ID_STRING   = 0x1,
        ID_NODE     = 0x2,
        ID_BONE     = 0x3,
        ID_VERTEX   = 0x4,
        ID_MATERIAL = 0x5,
        ID_MESH     = 0x6,
        ID_INDEX    = 0x7
    };

    enum
    {
        NODE_BLOCK     = FLAG::BLOCK | ID_NODE,
        MATERIAL_BLOCK = FLAG::BLOCK | ID_MATERIAL,
        MESH_BLOCK     = FLAG::BLOCK | ID_MESH,
        NODE_ARRAY     = FLAG::ARRAY | ID_NODE,
        BONE_ARRAY     = FLAG::ARRAY | ID_BONE,
        VERTEX_ARRAY   = FLAG::ARRAY | ID_VERTEX,
        INDEX_ARRAY    = FLAG::ARRAY | ID_INDEX,
        MESH_ARRAY     = FLAG::ARRAY | ID_MESH,
        STRING         = FLAG::CLASS | ID_STRING,
        NODE           = FLAG::CLASS | ID_NODE,
        BONE           = FLAG::CLASS | ID_BONE,
        VERTEX         = FLAG::CLASS | ID_VERTEX,
        MESH           = FLAG::CLASS | ID_MESH
    };

    struct Vertex
    {
        float    position[3];
        float    normal[3];
        float    uv[2];
        uint8_t  node_index;
        uint8_t  bone_indices[4];
        float    bone_weights[4];
        uint8_t  bone_count;
    };
}

#endif // GK_MDL_FORMAT_HPP

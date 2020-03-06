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
* |    MDL_Block    |   Node data       |
* |-----------------|-------------------|
* |    MDL_Block    |   Material data   |
* |-----------------|-------------------|
* |    MDL_Block    |   Mesh data       |
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
* |           MDL Matrix4F              |
* |-------------------------------------|
* |    Type         |   Description     |
* |-----------------|-------------------|
* |    UInt8        |   Signature       |
* |-----------------|-------------------|
* |    float[16]    |   Data            |
* |_________________|___________________|
*
* _______________________________________
* |     MDL Node Data Block Format      |
* |-------------------------------------|
* |    Type         |   Description     |
* |-----------------|-------------------|
* |    UInt8        |   Block Signature |
* |-----------------|-------------------|
* |    MDL_Array    |   Nodes           |
* |-----------------|-------------------|
* |    MDL_Array    |   Bones           |
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
* |    MDL_String   |   Texture         |
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
* |    MDL_Array    |   Vertices        |
* |-----------------|-------------------|
* |    MDL_Array    |   Mesh Data       |
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
* |    MDL_String   |   Name            |
* |-----------------|-------------------|
* |    MDL_String   |   Parent          |
* |-----------------|-------------------|
* |    MDL_Matrix4F |   Transform       |
* |_________________|___________________|
*
* _______________________________________
* |           MDL Bone Format           |
* |-------------------------------------|
* |    Type         |   Description     |
* |-----------------|-------------------|
* |    UInt8        |   Bone Signature  |
* |-----------------|-------------------|
* |    MDL_String   |   Name            |
* |-----------------|-------------------|
* |    MDL_Matrix4F |   Offset matrix   |
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
* |    float[4]     |  Bone weights     |
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
* |    MDL_String   |   Name            |
* |-----------------|-------------------|
* |    MDL_Array    |   Indices         |
* |_________________|___________________|
*
*/

namespace MDL
{
    enum
    {
        SIGNATURE   = 0x004C444D, // 'MDL'
        END_OF_FILE = 0x00464F45  // 'EOF'
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
        ID_INDEX    = 0x7,
        ID_MATRIX   = 0x8
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
        MESH           = FLAG::CLASS | ID_MESH,
        MATRIX         = FLAG::CLASS | ID_MATRIX
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

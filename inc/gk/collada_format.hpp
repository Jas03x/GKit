#ifndef COLLADA_FORMAT_HPP
#define COLLADA_FORMAT_HPP

#include <array>
#include <map>
#include <string>
#include <vector>

namespace Collada
{
    struct Param
    {
        enum NAME
        {
            X = 1,
            Y = 2,
            Z = 3,
            S = 4,
            T = 5,
            R = 6,
            G = 7,
            B = 8,
            A = 9,
            JOINT = 10,
            TRANSFORM = 11,
            WEIGHT = 12
        };

        enum TYPE
        {
            NAME     = 1,
            FLOAT    = 2,
            FLOAT4X4 = 3
        };

        unsigned char name;
        unsigned char type;
    };

    struct Accessor
    {
        const std::string* source;
        unsigned int count;
        unsigned int stride;
        
        unsigned int num_params;
        std::array<Param, 4> params;
    };

    struct Technique
    {
        Accessor accessor;
    };

    struct SourceArray
    {
        enum 
        {
            FLOAT = 1,
            NAME  = 2
        };

        const std::string* id;
        unsigned int count;
        unsigned int type;

        union
        {
            float* float_array;
            std::string* name_array;
        };
    };

    struct Source
    {
        const std::string* id;
        SourceArray array;
        Technique technique_common;
    };

    struct Input
    {
        enum
        {
            POSITION = 1,
            VERTEX = 2,
            NORMAL = 3,
            TEXCOORD = 4,
            COLOR = 5,
            JOINT = 6,
            INV_BIND_MATRIX = 7,
            WEIGHT = 8
        };

        const std::string* source;
        unsigned int offset;
        unsigned int set;
        unsigned int semantic;
    };

    struct VertexArray
    {
        const std::string* id;
        Input input;
        Source* source;

        ~VertexArray();
    };

    struct TriangleArray
    {
        const std::string* material;
        unsigned int count;
        unsigned short* indices;
        
        unsigned int num_inputs;
        std::array<Input, 4> inputs;

        ~TriangleArray();
    };

    struct Mesh
    {
        std::map<std::string, Source*> sources;
        VertexArray vertices;

        unsigned int num_triangle_arrays;
        TriangleArray* triangle_arrays;

        ~Mesh();
    };

    struct Joints
    {
        std::array<Input, 2> inputs;

        Source* names;
        Source* bind_poses;

        ~Joints();
    };

    struct VertexWeights
    {
        std::vector<unsigned short> v_count_array;
        std::vector<unsigned short> v_index_array;
        unsigned int count;

        std::array<Input, 2> inputs;
        Source* joints;
        Source* weights;

        ~VertexWeights();
    };

    struct Skin
    {
        // Mesh* source;
        const std::string* source;
        float bind_shape_matrix[16];
        std::map<std::string, Source*> sources;

        Joints joints;
        VertexWeights vertex_weights;

        ~Skin();
    };

    struct Geometry
    {
        const std::string* id;
        const std::string* name;

        Mesh mesh;
    };

    struct Controller
    {
        const std::string* id;
        const std::string* name;

        Skin skin;
    };

    struct Extra
    {
        struct
        {
            const std::string* profile;
            const std::string* layer;

            float roll;
            float tip_x;
            float tip_y;
            float tip_z;
        } technique;
    };

    struct Node
    {
        enum
        {
            NODE  = 1,
            JOINT = 2
        };

        const std::string* id;
        const std::string* sid;
        const std::string* name;
        unsigned int type;

        float scale[3];
        float rotation_x[4];
        float rotation_y[4];
        float rotation_z[4];
        float translation[3];

        Node* parent;

        Extra extra;
    };

    struct Image
    {
        const std::string* id;
        const std::string* name;

        struct
        {
            const std::string* file_name;
        } init_from;
    };

    struct VisualScene
    {
        const std::string* id;
        const std::string* name;

        std::map<std::string, Node*> nodes;

        ~VisualScene();
    };
}

#endif // COLLADA_FORMAT_HPP
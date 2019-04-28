#include <stdio.h>

#include <map>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <gk/mdl_format.hpp>

typedef unsigned char byte;

class Buffer
{
private:
    std::vector<byte> buffer;

public:
     Buffer() { }
    ~Buffer() { }

    void* reserve(unsigned int size)
    {
        unsigned int curr = buffer.size();
        buffer.insert(buffer.end(), size, 0);
        return &buffer[curr];
    }

    void write(void* ptr, unsigned int size)
    {
        byte* data = reinterpret_cast<byte*>(ptr);
        buffer.insert(buffer.end(), data, data + size);
    }

    unsigned long size() { return buffer.size(); }

    bool flush(const char* path)
    {
        FILE* file = fopen(path, "wb");
        if(!file) {
            printf("Error: Could not open file [%s] for writing\n", path);
            return false;
        }

        if(fwrite(buffer.data(), 1, buffer.size(), file) != buffer.size()) {
            printf("Error: Could not flush data to file [%s]\n", path);
            return false;
        }

        fclose(file);
        return true;
    }
};

void CopyString(void* destination, const void* source, unsigned int max)
{
    char* d = reinterpret_cast<char*>(destination);
    const char* s = reinterpret_cast<const char*>(source);
    unsigned int len = 0;

    while(((*s) != 0) && (len < max))
    {
        *d = *s;
        d ++;
        s ++;
        len ++;
    }
    *d = 0;
}

template <typename T>
struct Vector2T
{
    union { T v[3]; struct { T x, y; }; };

    Vector2T() { x = y = 0; }
    Vector2T(T _x, T _y) : x(_x), y(_y) { }

    T&        operator[] (unsigned int i)       { return v[i]; }
    const T&  operator[] (unsigned int i) const { return v[i]; }

    bool operator < (const Vector2T& v) const {
        return (x < v.x) || ((x == v.x) && (y < v.y));
    }

    bool operator == (const Vector2T& v) const {
        return (x == v.x) && (y == v.y);
    }
};

template <typename T>
struct Vector3T
{
    union { T v[3]; struct { T x, y, z; }; };

    Vector3T() { x = y = z = 0; }
    Vector3T(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { }

    T&        operator[] (unsigned int i)       { return v[i]; }
    const T&  operator[] (unsigned int i) const { return v[i]; }

    bool operator < (const Vector3T& v) const {
        return (x < v.x) || ((x == v.x) && (y < v.y)) || ((x == v.x) && (y == v.y) && (z < v.z));
    }

    bool operator == (const Vector3T& v) const {
        return (x == v.x) && (y == v.y) && (z == v.z);
    }
};

template <typename T>
struct Vector4T
{
    union { T v[4]; struct { T x, y, z, w; }; };

    Vector4T() { x = y = z = w = 0; }
    Vector4T(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }

    T&        operator[] (unsigned int i)       { return v[i]; }
    const T&  operator[] (unsigned int i) const { return v[i]; }

    bool operator < (const Vector4T& v) const {
        return (x < v.x) || ((x == v.x) && (y < v.y)) ||((x == v.x) && (y == v.y) && (z < v.z)) || ((x == v.x) && (y == v.y) && (z == v.z) && (w < v.w));
    }

    bool operator == (const Vector4T& v) const {
        return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
    }
};

typedef Vector2T<float>          Vector2F;
typedef Vector3T<float>          Vector3F;
typedef Vector4T<float>          Vector4F;
typedef Vector4T<unsigned short> Vector4U;

struct Matrix4F
{
private:
    float v[4][4];

public:
    float*       operator[] (unsigned int i)       { return v[i]; }
    const float* operator[] (unsigned int i) const { return v[i]; }
};

struct Vertex
{
    Vector3F position;
    Vector3F normal;
    Vector2F uv;
    unsigned char node_index;
    Vector4U bone_indices;
    Vector4F bone_weights;
    unsigned char material;
    unsigned char bone_count;

    bool operator < (const Vertex& v) const
    {
        bool b0 = (position  < v.position);
        bool b1 = (position == v.position) && (normal  < v.normal);
        bool b2 = (position == v.position) && (normal == v.normal) && (uv  < v.uv);
        bool b3 = (position == v.position) && (normal == v.normal) && (uv == v.uv) && (bone_indices  < v.bone_indices);
        bool b4 = (position == v.position) && (normal == v.normal) && (uv == v.uv) && (bone_indices == v.bone_indices) && (bone_weights  < v.bone_weights);
        bool b5 = (position == v.position) && (normal == v.normal) && (uv == v.uv) && (bone_indices == v.bone_indices) && (bone_weights == v.bone_weights) && (material  < v.material);
        bool b6 = (position == v.position) && (normal == v.normal) && (uv == v.uv) && (bone_indices == v.bone_indices) && (bone_weights == v.bone_weights) && (material == v.material) && (bone_count  < v.bone_count);
        bool b7 = (position == v.position) && (normal == v.normal) && (uv == v.uv) && (bone_indices == v.bone_indices) && (bone_weights == v.bone_weights) && (material == v.material) && (bone_count == v.bone_count) && (node_index < v.node_index);
        return (b0 || b1 || b2 || b3 || b4 || b5 || b6 || b7);
    }
};

struct Triangle
{
    unsigned short indices[3];
};

struct Mesh
{
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
};

struct Node
{
    std::string name;
    std::string parent;
    Matrix4F offset_matrix;
};

struct Material
{
    Vector3F color;
    Vector3F specular;
    unsigned char texture_index;
};

struct Frame
{
    float time;
    Vector3F position;
    Vector4F rotation;
    Vector3F scale;
};

struct Animation
{
    unsigned int length;
    std::vector<Frame> frames;
};

struct Bone
{
    std::string name;
    Matrix4F offset_matrix;
    Animation animation;
};

template <typename T>
class Container
{
private:
    std::map<std::string, unsigned int> index_map;
    std::vector<T> list;

public:
    Container() { }

    int get_index(const std::string& str) const
    {
        std::map<std::string, unsigned int>::const_iterator it = index_map.find(str);
        if(it != index_map.end()) {
            return it->second;
        }
        return -1;
    }

    T* find(const std::string& str)
    {
        int i = get_index(str);
        return (i == -1) ? nullptr : &list[i];
    }

    unsigned int insert(const std::string& str)
    {
        unsigned int index = list.size();
        list.push_back(T());

        index_map[str] = index;
        return index;
    }

    unsigned int size() const { return list.size(); }
    T&       operator [] (unsigned int i)       { return list[i]; }
    const T& operator [] (unsigned int i) const { return list[i]; }
};

typedef Container<Mesh> MeshContainer;
typedef Container<Bone> BoneContainer;
typedef Container<Node> NodeContainer;
typedef std::vector<Material>  MaterialList;
typedef Container<std::string> TextureContainer;

void GetNodes(const aiNode* ai_node, NodeContainer& nodes)
{
    Node& node  = nodes[nodes.insert(std::string(ai_node->mName.C_Str()))];
    node.name   = std::string(ai_node->mName.C_Str());
    node.parent = std::string(ai_node->mParent != nullptr ? ai_node->mParent->mName.C_Str() : "None");
    
    aiMatrix4x4 matrix = ai_node->mTransformation;
    matrix = matrix.Transpose();
    memcpy(&node.offset_matrix[0][0], &matrix[0][0], sizeof(float) * 16);

    for(unsigned int i = 0; i < ai_node->mNumChildren; i++)
    {
        GetNodes(ai_node->mChildren[i], nodes);
    }
}

void LoadBones(const struct aiMesh* ai_mesh, Mesh& mesh, unsigned int vertex_offset, BoneContainer& bones)
{
    for(unsigned int b = 0; b < ai_mesh->mNumBones; b++)
    {
        const struct aiBone* ai_bone = ai_mesh->mBones[b];
        std::string name = std::string(ai_bone->mName.C_Str());

        int bone_index = bones.get_index(name);
        if(bone_index == -1)
        {
            bone_index = bones.insert(name);

            Bone* bone = &bones[bone_index];
            bone->name = name;
            
            aiMatrix4x4 offset_matrix = ai_bone->mOffsetMatrix;
            offset_matrix = offset_matrix.Transpose();
            memcpy(&bone->offset_matrix[0][0], &offset_matrix[0][0], sizeof(Matrix4F));
        }

        for(unsigned int w = 0; w < ai_bone->mNumWeights; w++)
        {
            const struct aiVertexWeight weight = ai_bone->mWeights[w];
            unsigned int id = weight.mVertexId ; //+ vertex_offset; TODO: CHECK IF THIS IS NEEDED. IT MOST LIKELY IS NOT NEEDED BECAUSE THE MESH __USES__ THE SAME VERTICES. OR COULD BE WRONG AND MIGHT NEED ANOTHER DATA STRUCTURE TO FIND SIMILAR VERTICES WITHOUT FACTORING IN BONE INDICES AND WEIGHTS....
            unsigned char& bone_count = mesh.vertices[id].bone_count;

            if(bone_count < 4)
            {
                mesh.vertices[id].bone_indices[bone_count] = bone_index;
                mesh.vertices[id].bone_weights[bone_count] = weight.mWeight;
                bone_count ++;
            }
            else { printf("Warning: Maximum bones per vertex exceeded in mesh [%s]: Vertex %u of %lu\n", ai_mesh->mName.C_Str(), id, mesh.vertices.size()); }
        }
    }
}

void LoadAnimations(const struct aiScene* ai_scene, BoneContainer& bones)
{
    for(unsigned int a = 0; a < ai_scene->mNumAnimations; a++)
    {
        const aiAnimation* ai_animation = ai_scene->mAnimations[a];

        for(unsigned int n = 0; n < ai_animation->mNumChannels; n++)
        {
            const aiNodeAnim* node_animation = ai_animation->mChannels[n];
            std::string name = std::string(node_animation->mNodeName.data);

            int bone_index = bones.get_index(name);
            if(bone_index == -1)
            {
                printf("Warning: Bone [%s] not found for animation node\n", name.c_str());
                continue;
            }

            if((node_animation->mNumPositionKeys != node_animation->mNumRotationKeys) ||
               (node_animation->mNumPositionKeys != node_animation->mNumScalingKeys))
            {
                printf("Warning: Animation position, rotation, scaling keys not equal\n");
                continue;
            }

            Animation& animation = bones[bone_index].animation;
            animation.length = node_animation->mNumPositionKeys;

            for(unsigned int k = 0; k < node_animation->mNumPositionKeys; k++)
            {
                if ((node_animation->mPositionKeys[k].mTime != node_animation->mScalingKeys[k].mTime) ||
                    (node_animation->mPositionKeys[k].mTime != node_animation->mRotationKeys[k].mTime))
                {
                    printf("Warning: Timing of animation keys out of sync\n");
                }

                const aiVector3D   t = node_animation->mPositionKeys[k].mValue;
                const aiVector3D   s = node_animation->mScalingKeys[k].mValue;
                const aiQuaternion r = node_animation->mRotationKeys[k].mValue;

                Frame frame = { 0 };
                frame.time = static_cast<float>(node_animation->mPositionKeys[k].mTime);
                frame.position = Vector3F(t.x, t.y, t.z);
                frame.rotation = Vector4F(r.x, r.y, r.z, r.w);
                frame.scale = Vector3F(s.x, s.y, s.z);
                
                animation.frames.push_back(frame);
            }
        }
    }
}

bool LoadMeshes(const struct aiScene* ai_scene, MeshContainer& mesh_container, const NodeContainer& nodes, BoneContainer& bones)
{
    // Since meshes can be composed of different parts, we need to track vertex offsets on a per mesh basis
    std::map<std::string, unsigned int> offset_map;

    for(unsigned int m = 0; m < ai_scene->mNumMeshes; m++)
    {
        const struct aiMesh* ai_mesh = ai_scene->mMeshes[m];
        std::string name = std::string(ai_mesh->mName.C_Str());

        Mesh* mesh = mesh_container.find(name);
        if(mesh == nullptr)
        {
            mesh = &mesh_container[mesh_container.insert(name)];
            mesh->name = name;
        }

        int node_index = nodes.get_index(name);
        if(node_index == -1) { printf("Warning: Node not found for mesh [%s]\n", name.c_str()); node_index = 0; }  

        for(unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
        {
            Vertex vertex;
            
            const aiVector3D& v  = ai_mesh->mVertices[i];
            const aiVector3D& n  = ai_mesh->mNormals[i];
            const aiVector3D& uv = ai_mesh->HasTextureCoords(0) ? ai_mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);

            vertex.position     = Vector3F( v.x,  v.y,  v.z);
            vertex.normal       = Vector3F( n.x,  n.y,  n.z);
            vertex.uv           = Vector2F(uv.x, uv.y);
            vertex.node_index   = node_index;
            vertex.material     = ai_mesh->mMaterialIndex;
            vertex.bone_count   = 0;
            vertex.bone_indices = Vector4U(0, 0, 0, 0);
            vertex.bone_weights = Vector4F(0, 0, 0, 0);

            mesh->vertices.push_back(vertex);
        }

        for(unsigned int f = 0; f < ai_mesh->mNumFaces; f++)
        {
            const aiFace& face = ai_mesh->mFaces[f];
            if(face.mNumIndices != 3) printf("WARNING: Non-triangle face found -- discarding\n");

            Triangle triangle = { 0 };
            triangle.indices[0] = static_cast<unsigned short>(face.mIndices[0]);
            triangle.indices[1] = static_cast<unsigned short>(face.mIndices[1]);
            triangle.indices[2] = static_cast<unsigned short>(face.mIndices[2]);
           
            mesh->triangles.push_back(triangle);
        }

        unsigned int& vertex_offset = offset_map[name];
        LoadBones(ai_mesh, *mesh, vertex_offset, bones);
        vertex_offset += ai_mesh->mNumVertices;
    }

    return true;
}

bool LoadMaterials(const struct aiScene* ai_scene, MaterialList& materials, TextureContainer& textures)
{
    for(unsigned int t = 0; t < ai_scene->mNumMaterials; t++)
    {
        const aiMaterial* ai_material = ai_scene->mMaterials[t];
        
        char texture_index = 0;
        aiColor4D color = aiColor4D(0);
        aiColor4D specular = aiColor4D(0);

        if(aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_DIFFUSE, &color) != AI_SUCCESS)
        {
            printf("Error while reading material!\n");
            return false;
        }

        if(aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_SPECULAR, &specular) != AI_SUCCESS)
        {
            printf("Error while reading specular component!\n");
            return false;
        }

        if(ai_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString ai_path;

            if (ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &ai_path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                std::string path = std::string(ai_path.C_Str());
                
                texture_index = textures.get_index(path);
                if(texture_index == -1)
                {
                    texture_index = textures.insert(path);
                    textures[texture_index] = path;
                }
            }
        }

        Material material = { };
        material.color = Vector3F(color.r, color.g, color.b);
        material.specular = Vector3F(specular.r, specular.g, specular.b);
        material.texture_index = static_cast<unsigned char>(texture_index);

        materials.push_back(material);
    }

    return true;
}

void WriteMaterials(Buffer& buffer, const MaterialList& materials, const TextureContainer& textures)
{
    MDL_Array* array = nullptr;
    
    array = reinterpret_cast<MDL_Array*>(buffer.reserve(sizeof(MDL_Array)));
    array->type   = TEXTURE_ARRAY;
    array->length = textures.size();

    for(unsigned int i = 0; i < textures.size(); i++)
    {
        const std::string& path = textures[i];

        MDL_Texture* data = reinterpret_cast<MDL_Texture*>(buffer.reserve(sizeof(MDL_Texture)));
        CopyString(data->path.string, path.data(), MDL_MAX_STR_LEN);
    }

    array = reinterpret_cast<MDL_Array*>(buffer.reserve(sizeof(MDL_Array)));
    array->type   = MATERIAL_ARRAY;
    array->length = materials.size();

    for(unsigned int i = 0; i < materials.size(); i++)
    {
        const Material& material = materials[i];

        MDL_Material* data = reinterpret_cast<MDL_Material*>(buffer.reserve(sizeof(MDL_Material)));
        memcpy(&data->color,    &material.color,    sizeof(Vector3F));
        memcpy(&data->specular, &material.specular, sizeof(Vector3F));
        data->texture_index = material.texture_index;
    }
}

void WriteNodes(Buffer& buffer, const NodeContainer& nodes)
{
    MDL_Array* array = reinterpret_cast<MDL_Array*>(buffer.reserve(sizeof(MDL_Array)));
    array->type   = NODE_ARRAY;
    array->length = nodes.size();

    for(unsigned int n = 0; n < nodes.size(); n++)
    {
        const Node& node = nodes[n];
        
        MDL_Node* data = reinterpret_cast<MDL_Node*>(buffer.reserve(sizeof(MDL_Node)));
        CopyString(data->name.string, node.name.data(), MDL_MAX_STR_LEN);
        CopyString(data->parent.string, node.parent.data(), MDL_MAX_STR_LEN);
        memcpy(&data->offset_matrix.values[0][0], &node.offset_matrix[0][0], sizeof(float) * 16);
    }
}

void WriteBones(Buffer& buffer, const BoneContainer& bones)
{
    MDL_Array* array = reinterpret_cast<MDL_Array*>(buffer.reserve(sizeof(MDL_Array)));
    array->type   = BONE_ARRAY;
    array->length = bones.size();

    for(unsigned int b = 0; b < bones.size(); b++)
    {
        const Bone& bone = bones[b];

        MDL_Bone* data = reinterpret_cast<MDL_Bone*>(buffer.reserve(sizeof(MDL_Bone)));
        CopyString(&data->name.string, bone.name.data(), MDL_MAX_STR_LEN);
        memcpy(&data->offset_matrix, &bone.offset_matrix, sizeof(Matrix4F));
        
        unsigned int num_frames = bone.animation.length;
        data->animation.num_frames = num_frames;

        if(num_frames > 0)
        {
            MDL_Frame* frame_data = reinterpret_cast<MDL_Frame*>(buffer.reserve(sizeof(MDL_Frame) * num_frames));
            
            for(unsigned int f = 0; f < num_frames; f++)
            {
                const Frame& frame = bone.animation.frames[f];
                
                frame_data[f].time = frame.time;
                memcpy(frame_data[f].position.values, &frame.position[0], sizeof(Vector3F));
                memcpy(frame_data[f].rotation.values, &frame.rotation[0], sizeof(Vector4F));
                memcpy(frame_data[f].scale.values,    &frame.scale[0],    sizeof(Vector3F));
            }
        }
    }
}

void WriteMeshes(Buffer& buffer, const MeshContainer& mesh_container)
{
    MDL_Array* array = reinterpret_cast<MDL_Array*>(buffer.reserve(sizeof(MDL_Array)));
    array->type   = MESH_ARRAY;
    array->length = mesh_container.size();

    typedef std::map<Vertex, unsigned int> VertexCache;

    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
    VertexCache vertex_cache;
    unsigned int cache_hit = 0;

    for(unsigned int m = 0; m < mesh_container.size(); m++)
    {
        vertices.clear();
        triangles.clear();
        vertex_cache.clear();
        cache_hit = 0;

        const Mesh& mesh = mesh_container[m];
        for(unsigned int t = 0; t < mesh.triangles.size(); t++)
        {
            const Triangle& triangle = mesh.triangles[t];
            triangles.push_back(Triangle());

            for(unsigned int i = 0; i < 3; i++)
            {
                unsigned int vertex_index = 0;
                const Vertex& vertex = mesh.vertices[triangle.indices[i]];
                VertexCache::const_iterator it = vertex_cache.find(vertex);

                if(it == vertex_cache.end())
                {
                    vertex_index = vertices.size();
                    vertices.push_back(vertex);
                    vertex_cache[vertex] = vertex_index;
                }
                else { cache_hit ++; vertex_index = it->second; }

                triangles.back().indices[i] = vertex_index;
            }
        }

        printf("Mesh %s:", mesh.name.c_str());
        printf("Saved %u vertices (%lu bytes)\n", cache_hit, cache_hit * sizeof(Vertex));
        printf("%lu vertices | %lu triangles -> %lu indices\n", vertices.size(), triangles.size(), triangles.size() * 3);

        MDL_Mesh* data = reinterpret_cast<MDL_Mesh*>(buffer.reserve(sizeof(MDL_Mesh)));
        CopyString(data->name.string, mesh.name.c_str(), MDL_MAX_STR_LEN);
        data->vertex_array_length   = vertices.size();
        data->triangle_array_length = triangles.size();
        
        byte* ptr = reinterpret_cast<byte*>(buffer.reserve(sizeof(MDL_Vertex) * data->vertex_array_length + sizeof(MDL_Triangle) * data->triangle_array_length));
        MDL_Vertex*   vertex_data   = reinterpret_cast<MDL_Vertex*>  (ptr);
        MDL_Triangle* triangle_data = reinterpret_cast<MDL_Triangle*>(ptr + data->vertex_array_length * sizeof(MDL_Vertex));

        for(unsigned int v = 0; v < data->vertex_array_length; v++)
        {
            memcpy(vertex_data[v].position.values,     &vertices[v].position[0],     sizeof(Vector3F));
            memcpy(vertex_data[v].normal.values,       &vertices[v].normal[0],       sizeof(Vector3F));
            memcpy(vertex_data[v].uv.values,           &vertices[v].uv[0],           sizeof(Vector2F));
            memcpy(vertex_data[v].bone_indices.values, &vertices[v].bone_indices[0], sizeof(Vector4U));
            memcpy(vertex_data[v].bone_weights.values, &vertices[v].bone_weights[0], sizeof(Vector4F));

            vertex_data[v].node_index = vertices[v].node_index;
            vertex_data[v].material   = vertices[v].material;
            vertex_data[v].bone_count = vertices[v].bone_count;

            /* 
            printf("((%f, %f, %f), (%f, %f, %f), (%f, %f), ((%hu, %f), (%hu, %f), (%hu, %f), (%hu, %f)), %hhu, %hhu, %hhu)\n",
                vertex_data[v].position.values[0], vertex_data[v].position.values[1], vertex_data[v].position.values[2],
                vertex_data[v].normal.values[0], vertex_data[v].normal.values[1], vertex_data[v].normal.values[2],
                vertex_data[v].uv.values[0], vertex_data[v].uv.values[1],
                vertex_data[v].bone_indices.values[0], vertex_data[v].bone_weights.values[0],
                vertex_data[v].bone_indices.values[1], vertex_data[v].bone_weights.values[1],
                vertex_data[v].bone_indices.values[2], vertex_data[v].bone_weights.values[2],
                vertex_data[v].bone_indices.values[3], vertex_data[v].bone_weights.values[3],
                vertex_data[v].node_index,
                vertex_data[v].material,
                vertex_data[v].bone_count
            );
            */
        }

        for(unsigned int t = 0; t < data->triangle_array_length; t++)
        {
            triangle_data[t].indices[0] = triangles[t].indices[0];
            triangle_data[t].indices[1] = triangles[t].indices[1];
            triangle_data[t].indices[2] = triangles[t].indices[2];
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage: mdl.exe [input] [output]\n");
        return -1;
    }

    Assimp::Importer* ai_importer = new Assimp::Importer();
    const struct aiScene* ai_scene = ai_importer->ReadFile(argv[1], aiProcess_Triangulate);

    if(!ai_scene)
    {
        printf("Error: Could not open file [%s] for reading\n", argv[1]);
        return -1;
    }

    BoneContainer bone_container;
    MeshContainer mesh_container;
    NodeContainer node_container;

    MaterialList materials;
    TextureContainer texture_map;

    GetNodes(ai_scene->mRootNode, node_container);
    LoadMeshes(ai_scene, mesh_container, node_container, bone_container);
    LoadMaterials(ai_scene, materials, texture_map);
    LoadAnimations(ai_scene, bone_container);

    delete ai_importer;

    Buffer buffer;
    
    MDL_Header* header = reinterpret_cast<MDL_Header*>(buffer.reserve(sizeof(MDL_Header)));
    header->signature  = MDL_SIGNATURE;

    WriteMaterials(buffer, materials, texture_map);
    WriteNodes (buffer, node_container);
    WriteBones (buffer, bone_container);
    WriteMeshes(buffer, mesh_container);

    printf("Writing %lu bytes to file [%s]\n", buffer.size(), argv[2]);

    if(!buffer.flush(argv[2])) {
        return -1;
    }
    
    return 0;
}

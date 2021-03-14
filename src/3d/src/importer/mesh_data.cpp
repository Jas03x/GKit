#include <gk/3d/importer/mesh_data.hpp>

bool MeshData::VertexComparator::operator()(const MeshData::Vertex& v0, const MeshData::Vertex& v1) const
{
    bool ret = false;
    #define CMP(a, b) if((a) < (b)) { ret = true; } else if((a) == (b))
    CMP(v0.position, v1.position)
    {
        CMP(v0.normal, v1.normal)
        {
            CMP(v0.uv, v1.uv)
            {
                CMP(v0.node_index, v1.node_index)
                {
                    CMP(v0.bone_count, v1.bone_count)
                    {
                        Vector4F v0_indices(v0.bone_indices[0], v0.bone_indices[1], v0.bone_indices[2], v0.bone_indices[3]);
                        Vector4F v1_indices(v1.bone_indices[0], v1.bone_indices[1], v1.bone_indices[2], v1.bone_indices[3]);
                        CMP(v0_indices, v1_indices)
                        {
                            for (unsigned int i = 0; i < 4; i++)
                            {
                                float a = v0.bone_weights[i];
                                float b = v1.bone_weights[i];
                                if (a < b)
                                {
                                    ret = true;
                                    break;
                                }
                                else if (b < a)
                                {
                                    ret = false;
                                    break;
                                }
                                else
                                {
                                    continue;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    #undef CMP
    return ret;
}


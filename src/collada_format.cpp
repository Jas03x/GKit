#include <gk/collada_format.hpp>

Collada::VertexArray::~VertexArray()
{
    delete this->source;
}

Collada::TriangleArray::~TriangleArray()
{
    delete this->indices;
}

Collada::Mesh::~Mesh()
{
    for(std::map<std::string, Source*>::const_iterator it = this->sources.begin(); it != this->sources.end(); it++)
    {
        delete it->second;
    }

    delete this->triangle_arrays;
}

Collada::Joints::~Joints()
{
    delete this->names;
    delete this->bind_poses;
}

Collada::VertexWeights::~VertexWeights()
{
    delete this->joints;
    delete this->weights;
}

Collada::Skin::~Skin()
{
    for(std::map<std::string, Source*>::const_iterator it = this->sources.begin(); it != this->sources.end(); it++)
    {
        delete it->second;
    }
}

Collada::VisualScene::~VisualScene()
{
    for(std::map<std::string, Node*>::const_iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
    {
        delete it->second;
    }
}

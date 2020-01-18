#include <gk/collada_format.hpp>

VertexArray::~VertexArray()
{
    delete this->source;
}

TriangleArray::~TriangleArray()
{
    delete this->indices;
}

Mesh::~Mesh()
{
    for(std::map<std::string, Source*>::const_iterator it = this->sources.begin(); it != this->sources.end(); it++)
    {
        delete it->second;
    }

    delete this->triangle_arrays;
}

Joints::~Joints()
{
    delete this->joints;
    delete this->bind_poses;
}

VertexWeights::~VertexWeights()
{
    delete this->joints;
    delete this->weights;
}

Skin::~Skin()
{
    for(std::map<std::string, Source*>::const_iterator it = this->sources.begin(); it != this->sources.end(); it++)
    {
        delete it->second;
    }
}

VisualScene::~VisualScene()
{
    for(std::map<std::string, Source*>::const_iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
    {
        delete it->second;
    }
}

#include <gk/3d/importer/collada_format.hpp>

Collada::TriangleArray::~TriangleArray()
{
    delete this->indices;
}

Collada::Mesh::~Mesh()
{
    for(unsigned int i = 0; i < this->source_array.size(); i++)
    {
        delete this->source_array[i];
    }

    delete[] this->triangle_arrays;
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

#include <gk/collada_importer.hpp>

#include <gk/collada_parser.hpp>

Collada::Importer::Importer()
{
}

Collada::Importer::~Importer()
{
}

bool Collada::Importer::Import(const char* path, MeshData& data)
{
    Parser parser;
    Importer importer;

    bool status = parser.parse(path);
    if(status)
    {
        status = importer.process_scene(parser.GetSceneLibrary(), data);
    }

    return status;
}

bool Collada::Importer::process_geometry(const Collada::Parser::GeometryLibrary& library, MeshData& data)
{
    bool status = true;

    return status;
}

bool Collada::Importer::process_controllers(const Collada::Parser::ControllerLibrary& library, MeshData& data)
{
    bool status = true;

    return status;
}

bool Collada::Importer::process_scene(const Collada::Parser::SceneLibrary& library, MeshData& data)
{
    bool status = true;

    if(library.size() != 1)
    {
        printf("error importing collada object: unsupported number of scenes\n");
        status = false;
    }

    const VisualScene* scene = library.begin()->second;
    for(std::map<std::string, Node*>::const_iterator it = scene->nodes.begin(); status && (it != scene->nodes.end()); it++)
    {
        status = process_node(it->second, data);
    }

    return status;
}

bool Collada::Importer::process_node(const Collada::Node* node, MeshData& data)
{
    bool status = true;
    data.nodes.push_back(MeshData::Node());
    
    MeshData::Node& n = data.nodes.back();
    n.name = *node->name;
    n.parent = node->parent == nullptr ? nullptr : *node->parent->name;
    n.scale = Vector3F(node->scale[0], node->scale[1], node->scale[2]);
    n.rotation = Vector3F(node->rotation_x[3], node->rotation_y[3], node->rotation_z[3]);
    n.translation = Vector3F(node->translation[0], node->translation[1], node->translation[2]);

    return status;
}

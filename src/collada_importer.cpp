#include <gk/collada_importer.hpp>

#include <stdio.h>

#include <gk/xml.hpp>

void indent(unsigned int level)
{
    for(unsigned int i = 0; i < level; i++)
    {
        printf("\t");
    }
}

void print_node(const XML::Node* node, unsigned int level)
{
    indent(level);
    printf("<%s", node->name.c_str());

    for(std::map<std::string, std::string>::const_iterator it = node->attributes.begin(); it != node->attributes.end(); it++)
    {
        printf(" %s=\"%s\"", it->first.c_str(), it->second.c_str());
    }

    if((node->text.size() == 0) && (node->children.size() == 0))
    {
        printf("/>\n");
    }
    else
    {
        printf(">\n");

        if(node->text.size() > 0)
        {
            indent(level + 1);
            printf("%s\n", node->text.c_str());
        }
        else
        {
            for(std::map<std::string, std::vector<XML::Node*>>::const_iterator it = node->children.begin(); it != node->children.end(); it++)
            {
                const std::vector<XML::Node*>& children = it->second;
                for(unsigned int i = 0; i < children.size(); i++)
                {
                    print_node(children[i], level + 1);
                }
            }
        }

        indent(level);
        printf("</%s>\n", node->name.c_str());
    }
}

ColladaImporter::ColladaImporter(const char* path)
{
    const XML* xml = XML::Read(path);
    (void) xml;

    print_node(xml->root, 0);
}


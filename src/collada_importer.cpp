#include <gk/collada_importer.hpp>

#include <stdio.h>

#include <gk/xml.hpp>

ColladaImporter::ColladaImporter(const char* path)
{
    const XML* xml = XML::Read(path);
    (void) xml;

    xml->root->print();
}


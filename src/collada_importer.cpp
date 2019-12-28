#include <gk/collada_importer.hpp>

#include <stdio.h>

#include <gk/string.hpp>
#include <gk/string_buffer.hpp>
#include <gk/xml.hpp>

struct source
{
    const std::string* id;
    unsigned int stride;
    unsigned int length;
    std::vector<float> array;
};

int parse_array(const std::string& text, std::vector<float>& destination)
{
    int n = 0;
    StringBuffer buffer;
    
    char c =0;
    for(unsigned int i = 0 ;; i++)
    {
        c = text[i];

        if(IS_NUM(c) || (c == '.'))
        {
            buffer.push_back(c);
        }
        else
        {
            n++;
            destination.push_back(buffer.to_float());
        }

        if(c == 0)
        {
            break;
        }
    }

    return n;
}

bool read_source(const XML::Node* node, source& src)
{
    bool status = true;
    const XML::Node* accessor = nullptr;

    src.id = node->find_attribute("id");
    status = (src.id != nullptr);

    if(status)
    {
        const XML::Node* common = node->find_child("technique_common");
        status = (common != nullptr);

        if(status)
        {
            accessor = common->find_child("accessor");
            status = (accessor != nullptr);
        }
    }

    if(status)
    {
        const std::string* length = accessor->find_attribute("count");
        status = (length != nullptr);

        if(status)
        {
            const std::string* stride = accessor->find_attribute("stride");
            status = (stride != nullptr);

            if(status)
            {
                src.length = std::stoi(*length);
                src.stride = std::stoi(*stride);

                status = (src.length > 0) && (src.stride > 0);
            }
        }
    }

    if(status)
    {
        const XML::Node* array = node->find_child("float_array");
        status = (array != nullptr);

        if(status)
        {
            const std::string* p_count = array->find_attribute("count");
            status = (p_count != nullptr);

            if(status)
            {
                int count = std::stoi(*p_count);
                status = (count > 0) && (array->text.size() > 0);

                if(status)
                {
                    int n = parse_array(array->text, src.array);
                    (void) n;
                }
            }
        }
    }

    return status;
}

ColladaImporter::ColladaImporter(const char* path)
{
    const XML* xml = XML::Read(path);
    xml->root->print();
}


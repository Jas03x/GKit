#ifndef GK_XML_HPP
#define GK_XML_HPP

#include <cstdint>

#include <map>
#include <string>

class XML
{
public:
    struct Node
    {
        std::string name;
        std::map<std::string, std::string> attributes;

        std::string text;
        std::map<std::string, Node*> children;
    };

private:
    XML(Node* node);
    ~XML();

public:
    Node* root;

    static const XML* Read(const char* path);
};

#endif // GK_XML_HPP

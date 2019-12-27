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
    public:
        std::string name;
        std::map<std::string, std::string> attributes;

        std::string text;
        std::map<std::string, Node*> children;
    
    public:
        Node();
        ~Node();
    };

private:
    XML(Node* node);
    ~XML();

public:
    Node* root;

    const XML* Read(const char* path);
};

#endif // GK_XML_HPP

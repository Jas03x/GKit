#ifndef GK_XML_HPP
#define GK_XML_HPP

#include <cstdint>

#include <map>
#include <string>
#include <vector>

class XML
{
public:
    struct Node;
    typedef std::map<std::string, std::string> AttributeMap;
    typedef std::vector<Node*> ChildList;
    typedef std::map<std::string, ChildList> ChildMap;

    struct Node
    {
        std::string name;
        AttributeMap attributes;

        std::string text;
        ChildMap children;

        void print();

        const std::string* find_attribute(const std::string& key) const;
        
        const Node* find_child(const std::string& key) const;
        const ChildList* find_children(const std::string& key) const;

        ~Node();
    };

private:
    XML(Node* node);

public:
    Node* root;

    static const XML* Read(const char* path);
    ~XML();
};

#endif // GK_XML_HPP

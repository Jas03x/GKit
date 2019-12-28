#include <gk/xml.hpp>


#include <string.h>
#include <vector>

#include <gk/file.hpp>

#define IS_SPACE(x) (((x) == ' ') || ((x) == '\t') || ((x) == '\n'))

#define IS_ALPHA(x) ((((x) >= 'a') && ((x) <= 'z')) || (((x) >= 'A') && ((x) <= 'Z')))
#define IS_NUM(x) (((x) >= '0') && ((x) <= '9'))
#define IS_ALPHA_NUM(x) (IS_ALPHA(x) || IS_NUM(x) || ((x) == '_'))

class StringBuffer : public std::vector<char>
{
public:
    StringBuffer() : std::vector<char>()
    {
        this->reserve(128);
    }

    std::string to_string()
    {
        std::string str(this->data(), this->size());
        this->clear();
        return str;
    }
};

class XML_Reader
{
private:
    const std::string& m_Text;

    unsigned int m_Index;
    unsigned int m_CharPos;
    unsigned int m_LinePos;

    StringBuffer m_Buffer;

private:
    XML_Reader(const std::string& data) : m_Text(data)
    {
        m_Index = 0;
        m_CharPos = 0;
        m_LinePos = 0;
    }

    char peek(unsigned int offset)
    {
        return m_Text[m_Index + offset];
    }

    char pop()
    {
        char c = m_Text[m_Index++];
        if(c == '\n') {
            m_LinePos++;
            m_CharPos = 0;
        } else {
            m_CharPos++;
        }
        return c;
    }

    void error(const char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        printf("error (line %u char %u): ", m_LinePos, m_CharPos);
        vprintf(msg, args);
        va_end(args);
    }

    bool read_identifier(std::string& str)
    {
        bool status = true;

        char c = 0;
        while(status)
        {
            c = peek(0);
            if(IS_ALPHA_NUM(c) || (c == ':'))
            {
                pop();
                m_Buffer.push_back(c);
            }
            else
            {
                break;
            }
        }

        if(status)
        {
            if(m_Buffer.size() > 0)
            {
                str = m_Buffer.to_string();
            }
            else
            {
                status = false;
                error("expected identifier\n");
            }
        }

        return status;
    }

    bool read_value(std::string& str)
    {
        bool status = true;

        if(pop() != '"')
        {
            status = false;
            error("expected '\"'\n");
        }

        char c = 0;
        while(status)
        {
            c = pop();
            if(c != '"')
            {
                m_Buffer.push_back(c);
            }
            else
            {
                break;
            }
        }

        if(status)
        {
            if(m_Buffer.size() > 0)
            {
                str = m_Buffer.to_string();
            }
            else
            {
                status = false;
                error("expected string\n");
            }
        }

        return status;
    }

    bool read_text(std::string& str)
    {
        bool status = true;

        while(status)
        {
            if(peek(0) != '<')
            {
                m_Buffer.push_back(pop());
            }
            else
            {
                break;
            }
        }

        if(status)
        {
            if(m_Buffer.size() > 0)
            {
                str = m_Buffer.to_string();
            }
            else
            {
                status = false;
                error("expected text\n");
            }
        }

        return status;
    }

    enum LOOK_AHEAD
    {
        INVALID = 0,
        EMPTY   = 1,
        TEXT    = 2,
        BODY    = 3
    };

    LOOK_AHEAD lookahead()
    {
        LOOK_AHEAD status = LOOK_AHEAD::INVALID;

        char c = 0;
        for(unsigned int i = m_Index; i < m_Text.size(); i++)
        {
            c = m_Text[i];
            if(IS_SPACE(c))
            {
                continue;
            }
            else
            {
                if(c == '<')
                {
                    if(m_Text[i + 1] == '/')
                    {
                        status = LOOK_AHEAD::EMPTY;
                    }
                    else
                    {
                        status = LOOK_AHEAD::BODY;
                    }
                }
                else
                {
                    status = LOOK_AHEAD::TEXT;
                }
                break;
            }
        }

        return status;
    }

    XML::Node* read_node()
    {
        bool status = true;
        XML::Node* node = new XML::Node();

        std::string attr_name;
        std::string attr_value;

        char c = 0;
        unsigned int state = 1;

        while(status && (state != 0))
        {
            c = peek(0);

            if(m_Index >= m_Text.size())
            {
                status = false;
                printf("unexpected eof\n");
            }
            else if(IS_SPACE(c))
            {
                pop();
                continue;
            }
            else
            {
                switch(state)
                {
                    case 1: // expecting '<'
                    {
                        if(pop() == '<')
                        {
                            state = 2;
                        }
                        else
                        {
                            status = false;
                            error("expected '<'\n");
                        }
                        break;
                    }

                    case 2: // expecting node name
                    {
                        status = read_identifier(node->name);
                        if(status)
                        {
                            state = 3;
                        }
                        break;
                    }

                    case 3: // expecting attributes or end of node
                    {
                        if(c == '/')
                        {
                            pop();
                            state = 11;
                        }
                        else if(c == '>')
                        {
                            pop();

                            switch(lookahead())
                            {
                                case LOOK_AHEAD::EMPTY:
                                {
                                    state = 8;
                                    break;
                                }

                                case LOOK_AHEAD::TEXT:
                                {
                                    state = 8;
                                    status = read_text(node->text);
                                    break;
                                }

                                case LOOK_AHEAD::BODY:
                                {
                                    state = 7;
                                    break;
                                }

                                default:
                                {
                                    error("invalid node body\n");
                                    break;
                                }
                            }
                        }
                        else
                        {
                            status = read_identifier(attr_name);
                            if(status)
                            {
                                state = 4;
                            }
                        }
                        break;
                    }

                    case 4: // expecting '='
                    {
                        if(pop() == '=')
                        {
                            state = 5;
                        }
                        else
                        {
                            status = false;
                            error("expected '='\n");
                        }
                        break;
                    }

                    case 5: // expecting string value
                    {
                        status = read_value(attr_value);
                        if(status)
                        {
                            state = 3;
                            node->attributes[attr_name] = attr_value;
                        }
                        break;
                    }

                    case 6: // expecting '>' for end of node
                    {
                        if(pop() == '>')
                        {
                            state = 0;
                        }
                        else
                        {
                            status = false;
                            error("expected '>'\n");
                        }
                        break;
                    }

                    case 7: // expecting node body
                    {
                        if(c == '<')
                        {
                            if(peek(1) == '/')
                            {
                                state = 8;
                            }
                            else
                            {
                                XML::Node* child = read_node();
                                
                                if(child != nullptr)
                                {
                                    node->children[child->name] = child;
                                }
                                else
                                {
                                    status = false;
                                }
                                
                            }
                        }
                        else
                        {
                            status = false;
                            error("expected '<'\n");
                        }
                        break;
                    }

                    case 8: // expecting '<' for end tag
                    {
                        if(pop() == '<')
                        {
                            state = 9;
                        }
                        else
                        {
                            status = false;
                            error("expected '<'\n");
                        }
                        break;
                    }

                    case 9: // expecting '/' for end tag
                    {
                        if(pop() == '/')
                        {
                            state = 10;
                        }
                        else
                        {
                            status = false;
                            error("expected '/'\n");
                        }
                        break;
                    }

                    case 10: // expecting name for end tag
                    {
                        std::string name;
                        status = read_identifier(name);

                        if(status)
                        {
                            if(node->name == name)
                            {
                                state = 11;
                            }
                            else
                            {
                                status = false;
                                error("invalid xml structure\n");
                            }
                        }

                        break;
                    }

                    case 11:
                    {
                        if(pop() == '>')
                        {
                            state = 0;
                        }
                        else
                        {
                            status = false;
                            error("expected '>'\n");
                        }
                        break;
                    }

                    default:
                    {
                        status = false;
                        printf("invalid state\n");
                        break;
                    }
                }
            }
        }

        if(!status)
        {
            delete node;
            node = nullptr;
        }

        return node;
    }

public:
    static XML::Node* Read(const char* path)
    {
        XML::Node* root = nullptr;

        std::string file_data = File::Read(path);
        const char* data = file_data.data();

        static const char* HDR_STR = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
        static const unsigned int HDR_LEN = 38;

        if(strncmp(data, HDR_STR, HDR_LEN) == 0)
        {
            data += HDR_LEN;

            XML_Reader reader(data);
            root = reader.read_node();
        }
        else
        {
            printf("unsupported or invalid xml file\n");
        }
        
        return root;
    }
};

XML::XML(Node* node)
{
    this->root = node;
}

XML::~XML()
{
    delete this->root;
}

const XML* XML::Read(const char* path)
{
    XML* xml = nullptr;

    XML::Node* root = XML_Reader::Read(path);
    if(root != nullptr)
    {
        xml = new XML(root);
    }
    else
    {
        printf("error reading xml file\n");
    }

    return xml;
}


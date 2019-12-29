#ifndef GK_STRING_BUFFER_HPP
#define GK_STRING_BUFFER_HPP

#include <string>
#include <vector>

class StringBuffer : public std::vector<char>
{
public:
    StringBuffer();

    int to_int();
    float to_float();
    std::string to_string();
};

#endif // GK_STRING_BUFFER_HPP

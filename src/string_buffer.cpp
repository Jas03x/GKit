#include <gk/string_buffer.hpp>

StringBuffer::StringBuffer() : std::vector<char>()
{
    this->reserve(128);
}

float StringBuffer::to_float()
{
    this->push_back(0);
    float f = atof(this->data());

    this->clear();
    return f;
}

std::string StringBuffer::to_string()
{
    std::string str(this->data(), this->size());
    this->clear();
    return str;
}


#include <gk/core/utility/string_buffer.hpp>

StringBuffer::StringBuffer()
{
    this->reserve(128);
}

int StringBuffer::to_int()
{
    this->push_back(0);
    int i = atoi(this->data());

    this->clear();
    return i;
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


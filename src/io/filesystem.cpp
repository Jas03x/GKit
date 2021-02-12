#include <gk/filesystem.hpp>

Filesystem::Path::Path(const std::string& path)
{
    size_t index = path.find_last_of('/');

    if (index == std::string::npos)
    {
        this->filename = path;
    }
    else
    {
        index++;
        this->directory = path.substr(0, index);
        this->filename = path.substr(index);
    }
}

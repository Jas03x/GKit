#ifndef GK_FILESYSTEM_HPP
#define GK_FILESYSTEM_HPP

#include <string>

namespace Filesystem
{
	struct Path
	{
		std::string directory;
		std::string filename;

		Path(const std::string& path);
	};
};

#endif // GK_FILESYSTEM_HPP
#ifndef GK_TEXTURE_MANAGER_HPP
#define GK_TEXTURE_MANAGER_HPP

#include <string>

#include <gk/texture.hpp>

class TextureManager
{
public:
	static void CreateInstance(const std::string& texture_directory);
	static void DeleteInstance();

	static Texture* Load(const std::string& texture);
};

#endif // GK_TEXTURE_MANAGER_HPP

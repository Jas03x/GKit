#ifndef GK_TEXTURE_MANAGER_HPP
#define GK_TEXTURE_MANAGER_HPP

#include <map>
#include <string>

#include <gk/core/graphics/texture.hpp>

class TextureManager
{
private:
    static TextureManager* Instance;
    
private:
    std::string m_TextureDirectory;
    std::map<std::string, Texture*> m_TextureMap;
    
private:
    TextureManager(const std::string& texture);
    ~TextureManager();
    
public:
	static TextureManager* CreateInstance(const std::string& texture_directory);
	static void DeleteInstance();
    
    static TextureManager* GetInstance();
    
public:
	Texture* Load(const std::string& texture);
};

#endif // GK_TEXTURE_MANAGER_HPP

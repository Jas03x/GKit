#include <gk/3d/core/texture_manager.hpp>

#include <cassert>

#include <gk/3d/config/config.hpp>
#include <gk/core/importer/bitmap.hpp>

TextureManager* TextureManager::Instance = nullptr;

TextureManager::TextureManager(const std::string& texture_directory)
{
    m_TextureDirectory = texture_directory;
}

TextureManager::~TextureManager()
{
    for (auto it = m_TextureMap.begin(); it != m_TextureMap.end(); it++)
    {
        delete it->second;
    }
}

TextureManager* TextureManager::CreateInstance(const std::string& texture_directory)
{
	assert(Instance == nullptr);
	if (Instance == nullptr)
	{
		Instance = new TextureManager(texture_directory);
	}
    return Instance;
}

void TextureManager::DeleteInstance()
{
	assert(Instance != nullptr);

	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

TextureManager* TextureManager::GetInstance()
{
    return Instance;
}

Texture* TextureManager::Load(const std::string& texture)
{
    Texture* ptr = nullptr;
    auto it = m_TextureMap.find(texture);

    if (it != m_TextureMap.end())
    {
        ptr = it->second;
    }
    else
    {
        Bitmap image((m_TextureDirectory + texture).c_str());
        ptr = new Texture(image.has_alpha ? GFX_RGBA : GFX_RGB, image.width, image.height, GFX_TYPE_UNSIGNED_BYTE, image.pixels, GFX_LINEAR, GFX_CLAMP_TO_EDGE);

        m_TextureMap[texture] = ptr;
    }

    return ptr;
}

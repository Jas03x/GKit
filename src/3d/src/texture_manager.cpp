#include <gk/3d/texture_manager.hpp>

#include <cassert>

#include <gk/3d/config.hpp>
#include <gk/core/importer/bitmap.hpp>

TextureManager* TextureManager::Instance = nullptr;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    for (auto it = m_TextureMap.begin(); it != m_TextureMap.end(); it++)
    {
        delete it->second;
    }
}

TextureManager* TextureManager::CreateInstance()
{
	assert(Instance == nullptr);
	if (Instance == nullptr)
	{
		Instance = new TextureManager();
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
        Bitmap image(texture.c_str());
        ptr = new Texture(image, GFX_LINEAR, GFX_CLAMP_TO_EDGE);

        m_TextureMap[texture] = ptr;
    }

    return ptr;
}

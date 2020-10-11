#include <gk/texture_manager.hpp>

#include <cassert>

#include <map>

#include <gk/bitmap.hpp>
#include <gk/config.hpp>

class TextureManagerInstance : public TextureManager
{
private:
	std::string m_TextureDirectory;
	std::map<std::string, Texture*> m_TextureMap;

public:
	TextureManagerInstance(const std::string& texture_directory)
	{
		m_TextureDirectory = texture_directory;
	}

	~TextureManagerInstance()
	{
		for (auto it = m_TextureMap.begin(); it != m_TextureMap.end(); it++)
		{
			delete it->second;
		}
	}

	Texture* Load(const std::string& texture)
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
}* Instance = nullptr;

void TextureManager::CreateInstance(const std::string& texture_directory)
{
	assert(Instance == nullptr);

	if (Instance == nullptr)
	{
		Instance = new TextureManagerInstance(texture_directory);
	}
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

Texture* TextureManager::Load(const std::string& texture)
{
	assert(Instance != nullptr);

	return Instance->Load(texture);
}
